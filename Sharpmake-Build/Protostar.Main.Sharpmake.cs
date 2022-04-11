using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Reflection;
using Sharpmake;
using static Sharpmake.Solution.Configuration;

[module: Include("../Protostar-Engine/Plugins/*/*/Protostar.*.Sharpmake.cs")]
[module: Include("../Protostar-Engine/Protostar.Engine.Sharpmake.cs")]

namespace Protostar
{
    [Generate]
    public abstract class BaseProject : Project
    {
        public BaseProject()
        {
            IsFileNameToLower = false;
            IsTargetFileNameToLower = false;
            AddTargets(new Target(
            Platform.win64,
            Main.CustomArguments.DevEnvArgument,
            Optimization.Debug | Optimization.Release,
            OutputType.Dll));
        }

        [Configure]
        public virtual void ConfigureAll(Configuration configuration, Target target)
        {
            configuration.IncludePaths.Add(Path.Combine("[project.SharpmakeCsPath]", "Source", "Public"));
            configuration.Options.Add(Options.Vc.Compiler.CppLanguageStandard.CPP20);
            configuration.Options.Add(Options.Vc.Compiler.RTTI.Disable);
            configuration.Options.Add(Options.Vc.General.WindowsTargetPlatformVersion.v10_0_22000_0);
            configuration.Options.Add(Options.Vc.General.PlatformToolset.v143);
            configuration.IntermediatePath = Path.Combine(MainSolution.GeneralIntermediateFolder, target.Optimization.ToString(), Name);
            configuration.TargetPath = Path.Combine(MainSolution.GeneralOutputPathFolder, target.Optimization.ToString(), Name);
            configuration.Defines.Add($"{Name}_EXPORTS");
        }
    }

    [Generate]
    public abstract class Module : BaseProject
    {
        public Module()
        {

        }
        public override void ConfigureAll(Configuration configuration, Target target)
        {
            base.ConfigureAll(configuration, target);
            configuration.Output = Configuration.OutputType.Dll;
            configuration.SolutionFolder = $"/Protostar-Engine/Plugins/{PluginName}";
        }
        protected string PluginName { get; set; }
    }

    [Generate]
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
                    throw new Exception("Protostar is only supporting Visual Studio 2022 yet");
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
