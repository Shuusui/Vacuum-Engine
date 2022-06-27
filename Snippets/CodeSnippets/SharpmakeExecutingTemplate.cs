using System.IO;
using Sharpmake;

namespace Protostar
{
    [Generate]
    public class {ModuleName} : ExecutingProject
    {
        public {ModuleName}()
        {
            
        }
        [Configure]
        public virtual void ConfigModule(Configuration configuration, Target target)
        {
            configuration.AddPublicDependency<MainCore>(target);
        }
    }
}