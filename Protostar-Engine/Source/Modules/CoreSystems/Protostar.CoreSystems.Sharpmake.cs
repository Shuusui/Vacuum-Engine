using System.IO;
using Sharpmake;

namespace Protostar
{
    [Generate]
    public class CoreSystems : Module
    {
        public CoreSystems()
        {
        }
        public override void ConfigureAll(Configuration configuration, Target target)
        {
            base.ConfigureAll(configuration, target);
            configuration.AddPublicDependency<MainCore>(target);
            configuration.AddPrivateDependency<CoreMath>(target);
        }
    }
}
