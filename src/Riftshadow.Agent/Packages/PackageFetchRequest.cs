namespace Riftshadow.Agent.Packages;

/// <summary>
/// A request for fetching packages, including from where to fetch, where
/// to save package data, and what to call the package.
/// </summary>
public class PackageFetchRequest
{
    public string ReleaseTag { get; set; } = string.Empty;
    public string PackageName { get; set; } = string.Empty;
    public string DownloadFromPath { get; set; } = string.Empty;
    public string DownloadToPath { get; set; } = string.Empty;
}