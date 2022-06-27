using System;
using System.Collections.Concurrent;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Reflection;
using Sharpmake;
using static Sharpmake.Solution.Configuration;

[module: Include("../Protostar-Engine/Source/Modules/*/Protostar.*.Sharpmake.cs")]
[module: Include("../Protostar-Engine-Test/Source/Modules/*/Protostar.*.Sharpmake.cs")]
[module: Include("../Protostar-Editor/Source/Modules/Protostar.*.Sharpmake.cs")]
[module: Include("../Protostar-Editor/Source/Modules/*/Protostar.*.Sharpmake.cs")]

namespace Protostar
{
    public abstract class BaseProject : Project
    {
        public BaseProject()
        {
            IsFileNameToLower = false;
            IsTargetFileNameToLower = false;
            SourceRootPath = Path.Combine("[project.SharpmakeCsPath]", "Source");
            AddTargets(new Target(
                Platform.win64,
                Main.CustomArguments.DevEnvArgument,
                Optimization.Debug | Optimization.Release,
                OutputType.Dll));
        }
        [Configure]
        public virtual void ConfigureAll(Configuration configuration, Target target)
        {
            configuration.Options.Add(Sharpmake.Options.Vc.General.CharacterSet.Unicode);

            AddSolutionFolders(configuration, "Protostar-Engine");
            AddSolutionFolders(configuration, "Protostar-Engine-Test");
            AddSolutionFolders(configuration, "Protostar-Editor");
            AddSolutionFolders(configuration, "Projects");

            SetConfiguration(configuration, target);
        }
        private void SetConfiguration(Configuration configuration, Target target)
        {
            configuration.IncludePaths.Add(Path.Combine("[project.SharpmakeCsPath]", "Source", "Public"));
            configuration.Options.Add(Options.Vc.Compiler.CppLanguageStandard.CPP20);
            configuration.Options.Add(Options.Vc.Compiler.RTTI.Disable);
            configuration.Options.Add(Options.Vc.General.WindowsTargetPlatformVersion.v10_0_22000_0);
            configuration.Options.Add(Options.Vc.General.PlatformToolset.v143);
            configuration.IntermediatePath = Path.Combine(MainSolution.GeneralIntermediateFolder, target.Optimization.ToString(), Name);
            configuration.TargetPath = Path.Combine(MainSolution.GeneralOutputPathFolder, target.Optimization.ToString(), Name);
            configuration.Defines.Add($"{Name}_API");
            configuration.ProjectPath = "[project.SharpmakeCsPath]";
        }
        private void AddSolutionFolders(Configuration configuration, string rootFolderName)
        {
            List<string> _filePaths = new List<string>(new string[] { rootFolderName });
            if(FindTypeFileName(Path.Combine(MainSolution.RootFolder, rootFolderName), ref _filePaths))
            {
                configuration.SolutionFolder = Path.Combine(_filePaths.ToArray());
            }
        }
        private bool FindTypeFileName(string currentPath, ref List<string> filePaths)
        {
            string _searchPattern = $"Protostar.{GetType().Name}.Sharpmake.cs";
            string[] _files = Directory.GetFiles(currentPath, _searchPattern);
            if(_files.Length > 0)
            {
                filePaths.RemoveAt(filePaths.Count - 1);
                return true;
            }

            foreach (string _directory in Directory.GetDirectories(currentPath))
            {
                List<string> _tempList = new List<string>(filePaths)
                {
                    Path.GetFileName(_directory)
                };
                if (FindTypeFileName(_directory, ref _tempList))
                {
                    filePaths = _tempList;
                    return true;
                }
            }
            return false;
        }
    }
    public abstract class Module : BaseProject
    {
        public Module()
        {
            
        }
        public override void ConfigureAll(Configuration configuration, Target target)
        {
            base.ConfigureAll(configuration, target);
            configuration.Output = Configuration.OutputType.Dll;
        }
    }
    public abstract class ExecutingProject : BaseProject
    {
        public override void ConfigureAll(Configuration configuration, Target target)
        {
            base.ConfigureAll(configuration, target);
            configuration.Output = Configuration.OutputType.Exe;
        }
    }
    [Generate]
    public class MainSolution : Solution
    {
        public MainSolution()
        {
            Name = "Protostar";
            IsFileNameToLower = false;
            AddTargets(new Target(
                Platform.win64,
                Main.CustomArguments.DevEnvArgument,
                Optimization.Debug | Optimization.Release,
                OutputType.Dll
                ));
        }
        [Configure]
        public void ConfigureAll(Configuration configuration, Target target)
        {
            RootFolder = Path.Combine("[solution.SharpmakeCsPath]", @"..\");
            GeneralIntermediateFolder = Path.Combine(
                RootFolder,
                "intermediate") ;

            GeneralOutputPathFolder = Path.Combine(
                RootFolder,
                "bin");

            configuration.SolutionPath = Path.Combine("[solution.SharpmakeCsPath]", @"..\");
            configuration.SolutionFileName = "[solution.Name]";
            Type[] _moduleTypes = Assembly.GetAssembly(typeof(Module)).GetTypes();
            HashSet<Type> _executingProjects = new HashSet<Type>();
            foreach (Type _type in _moduleTypes)
            {
                if (_type.IsClass && !_type.IsAbstract && _type.IsSubclassOf(typeof(Module)))
                {
                    configuration.AddProject(_type, target);
                }
                else if (_type.IsClass && !_type.IsAbstract && _type.IsSubclassOf(typeof(ExecutingProject)))
                {
                    _executingProjects.Add(_type);
                    configuration.AddProject(_type, target);
                    IncludedProjectInfo _exeProject = configuration.GetProject(_type);
                    configuration.StartupProject = _exeProject;
                }
            }
        }
        public static string GeneralIntermediateFolder { get; private set; }
        public static string GeneralOutputPathFolder { get; private set; }
        public static string RootFolder { get; private set; }
    }
    public class ProtostarArguments
    {
        public DevEnv DevEnvArgument = DevEnv.VisualStudio;

        [CommandLine.Option("devenv", @"Dev Env: ex: /devenv(22)")]
        public void CommandLineSetDevEnv(int value)
        {
            switch (value)
            {
                case 22:
                    DevEnvArgument = DevEnv.vs2022;
                    break;
                default:
                    throw new Exception($"Protostar is not supporting Visual Studio {value}");
            }
        }
    }
    public static class Main
    {
        public static ProtostarArguments CustomArguments = new ProtostarArguments();
        [Sharpmake.Main]
        public static void SharpmakeMain(Arguments arguments)
        {
            CommandLine.ExecuteOnObject(CustomArguments);
            
            arguments.Generate<MainSolution>();
        }
    }
}
