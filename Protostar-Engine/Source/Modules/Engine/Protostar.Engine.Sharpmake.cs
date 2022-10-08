using System.IO;
using Sharpmake;

namespace Protostar
{
    [Generate]
    public class Engine : Module
    {
        public Engine()
        {

        }
        public override void ConfigureAll(Configuration configuration, Target target)
        {
            base.ConfigureAll(configuration, target);
            configuration.AddPublicDependency<MainCore>(target);
            configuration.AddPrivateDependency<DX12>(target);
            configuration.AddPrivateDependency<RHI>(target);
        }
    }
}
