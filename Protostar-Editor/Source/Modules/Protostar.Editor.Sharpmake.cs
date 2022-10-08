using System.IO;
using Sharpmake;

namespace Protostar
{
    [Generate]
    public class Editor : ExecutingProject
{
        public Editor()
        {
        }
        public override void ConfigureAll(Configuration configuration, Target target)
        {
            base.ConfigureAll(configuration, target);
            configuration.AddPrivateDependency<MainCore>(target);
            configuration.AddPrivateDependency<Engine>(target);
            configuration.Options.Add(Options.Vc.Linker.SubSystem.Windows);
        }
    }
}
