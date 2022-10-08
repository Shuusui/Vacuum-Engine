using System.IO;
using Sharpmake;

namespace Protostar
{
    [Generate]
    public class CoreMath : Module
    {
        public CoreMath()
        {
        }
        public override void ConfigureAll(Configuration configuration, Target target)
        {
            base.ConfigureAll(configuration, target);
            configuration.AddPublicDependency<MainCore>(target);
        }
    }
}