using System.IO;
using Sharpmake;

namespace Protostar
{
    [Generate]
    public class DX12 : Module
    {
        public DX12()
        {
            
        }
        public override void ConfigureAll(Configuration configuration, Target target)
        {
            base.ConfigureAll(configuration, target);
            configuration.AddPublicDependency<MainCore>(target);
            configuration.AddPublicDependency<Renderer>(target);
            configuration.LibraryFiles.Add("D3D12.lib");
            configuration.LibraryFiles.Add("DXGI.lib");
            configuration.LibraryFiles.Add("dxguid.lib");

            configuration.Options.Add(new Options.Vc.Compiler.DisableSpecificWarnings(new string[]
            {
                "4840"
            }));
        }
    }
}
