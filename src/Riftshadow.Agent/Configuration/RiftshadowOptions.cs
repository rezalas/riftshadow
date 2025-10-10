
namespace Riftshadow.Agent.Configuration;

/// <summary>
/// The configuration options for the Riftshadow agent.
/// </summary>
public class RiftshadowAgentOptions
{
    /// <summary>
    /// The name of the configuration section for Riftshadow options.
    /// </summary>
    public const string SectionName = "RiftshadowAgent";

    /// <summary>
    /// The path to download release assets to before installation.
    /// </summary>
#if LINUX
    public string ReleaseDownloadPath { get; set; } = "/usr/rift/releases";
#else
    public string ReleaseDownloadPath { get; set; } = $"{Environment.GetFolderPath(Environment.SpecialFolder.LocalApplicationData)}/Riftshadow/releases";
#endif

    /// <summary>
    /// The beginning of the package name for the riftshadow game
    /// from the release assets.
    /// </summary>
    public string GamePackagePrefix { get; set; } = "Riftshadow";

    /// <summary>
    /// The target installation path for the Riftshadow game.
    /// </summary>
#if LINUX
    public string GameInstallPath { get; set; } = "/usr/rift";
#else
    public string GameInstallPath { get; set; } = $"{Environment.GetFolderPath(Environment.SpecialFolder.LocalApplicationData)}/Riftshadow/game";

    /// <summary>
    /// The beginning of the package name for the riftshadow agent
    /// from the release assets.
    /// </summary>
    public string AgentPackagePrefix { get; set; } = "Riftshadow.Agent";

    /// <summary>
    /// The target installation path for the Riftshadow agent.
    /// </summary>
#if LINUX
    public string AgentInstallPath { get; set; } = "/usr/riftagent";
#else
    public string AgentInstallPath { get; set; } = $"{Environment.GetFolderPath(Environment.SpecialFolder.LocalApplicationData)}/Riftshadow/agent";

    /// <summary>
    /// The cron expression for how often to scan for new releases.
    /// Defaults to every minute.
    /// </summary>
    public string ReleaseScanCron { get; set; } = "0 0/1 * * * ?";
}