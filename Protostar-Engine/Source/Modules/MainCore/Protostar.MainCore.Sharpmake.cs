using System.IO;
using Sharpmake;

namespace Protostar
{
    [Generate]
    public class MainCore : Module
    {
        public MainCore()
        {
        }
        public override void ConfigureAll(Configuration configuration, Target target)
        {
            base.ConfigureAll(configuration, target);
        }
    }
}
