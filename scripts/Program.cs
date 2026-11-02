using System;
using System.IO;
using UAssetAPI;
using UAssetAPI.UnrealTypes;

class Program
{
    static void Main(string[] args)
    {
        var asset = new UAsset(args[0], EngineVersion.VER_UE4_27);
        var json = asset.SerializeJson(true);
        File.WriteAllText(args[1], json);
        Console.WriteLine("wrote " + args[1] + " bytes=" + json.Length);
    }
}
