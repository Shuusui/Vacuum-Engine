using System.IO;
using Sharpmake;

namespace Protostar
{
    [Generate]
    public class RHI : Module
    {
        public RHI()
        {
            
        }
        public override void ConfigureAll(Configuration configuration, Target target)
        {
            base.ConfigureAll(configuration, target);
            configuration.AddPublicDependency<MainCore>(target);
            configuration.AddPublicDependency<Renderer>(target);
            configuration.AddPrivateDependency<FileSystem>(target);
        }
    }
}
