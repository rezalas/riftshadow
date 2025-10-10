namespace Riftshadow.Agent.Packages;

public class PackageFetchRequest
{
    public string ReleaseTag { get; set; } = string.Empty;
    public string PackageName { get; set; } = string.Empty;
    public string DownloadFromPath { get; set; } = string.Empty;    
    public string DownloadToPath { get; set; } = string.Empty;
}