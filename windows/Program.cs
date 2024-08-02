using System;
using System.Runtime.InteropServices;
using System.Text;

namespace csexample
{
    internal class Program
    {
        /* PInvoke jmentrance */
        [DllImport("jmentrance.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.Cdecl)]
        internal static extern int jmentrance_decrypt([MarshalAs(UnmanagedType.LPStr)] string pubkey,
            [MarshalAs(UnmanagedType.LPStr)] string cluster_id,
            [MarshalAs(UnmanagedType.LPStr)] string sn,
            [MarshalAs(UnmanagedType.LPStr)] string input,
            [MarshalAs(UnmanagedType.LPStr)] StringBuilder output, int output_len);

        static void Main(string[] args)
        {
            /* JJS */
            string pubkey = "bad265420de99f9f78435d2207e44859ca4eba4af53650d50ed63466e3657225cf07967277e5093e25af511eeb9f1aabf61646db59df1e9722ad901851ffca3d";
            string clusterid = "ba24cc34";

            /* JMJ */
            /*
            string pubkey = "830d659aa4500b4fe5d0dd5c8e38e367a564a6fe846ed758b34884ee40dbfaefb5454b1f3b250eed2e52a93abbef99c5c142a9218f58198fda9da4e035ccdb3b";
            string clusterid = "ba5cee4e";
            */

            /* Output Buffer */
            StringBuilder output = new StringBuilder(45);

            /* HIMBARA */
            string sn = "7546000012122780"; /* Gunakan sn kartu untuk BCA */
            string data = "615870B6B93F44ABB07EEADCB581CBC6FFFFFFFFFFFF";
            int ret = jmentrance_decrypt(pubkey, clusterid, sn, data, output, 45);
            Console.WriteLine("HIMBARA: "+ret+" = "+output);

            /* BCA */
            sn = "AB56EE01"; /* Gunakan uuid kartu untuk BCA */
            data = "05A4B618A982AA55D61C1284684923E7FFFFFFFFFFFF";
            ret = jmentrance_decrypt(pubkey, clusterid, sn, data, output, 45);
            Console.WriteLine("BCA    : " + ret + " = " + output);
        }
    }
}
