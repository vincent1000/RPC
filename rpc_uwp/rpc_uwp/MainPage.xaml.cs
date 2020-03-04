using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices;
using System.Runtime.InteropServices.WindowsRuntime;
using Windows.Foundation;
using Windows.Foundation.Collections;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Controls.Primitives;
using Windows.UI.Xaml.Data;
using Windows.UI.Xaml.Input;
using Windows.UI.Xaml.Media;
using Windows.UI.Xaml.Navigation;

// https://go.microsoft.com/fwlink/?LinkId=402352&clcid=0x804 上介绍了“空白页”项模板

namespace rpc_uwp
{
    /// <summary>
    /// 可用于自身或导航至 Frame 内部的空白页。
    /// </summary>
    public sealed partial class MainPage : Page
    {
        [DllImport("Rpcrt4.dll", CharSet = CharSet.Auto)]
        public static extern int RpcBindingFromStringBinding(string StringBinding,out IntPtr Binding);

        [DllImport("Rpcrt4.dll", CharSet = CharSet.Auto)]
        public static extern int RpcBindingFree(ref IntPtr Binding);

        [DllImport("Rpcrt4.dll", CharSet = CharSet.Auto)]
        public static extern int RpcStringBindingCompose(string ObjUuid, string ProtSeq, string NetworkAddr, string EndPoint, string Options, out string StringBinding);
        
        public MainPage()
        {
            this.InitializeComponent();

            IntPtr bindingHandle = IntPtr.Zero;
            string pszStringBinding;
            RpcStringBindingCompose(
                        null,
                        "ncalrpc",
                        null,
                        "myname",
                        null,
                        out pszStringBinding);

            int ret = RpcBindingFromStringBinding(pszStringBinding, out bindingHandle);
            Console.WriteLine("Ret = " + ret);
        }
    }
}
