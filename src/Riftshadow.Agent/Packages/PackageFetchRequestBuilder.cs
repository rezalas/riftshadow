namespace Riftshadow.Agent.Packages;

public class PackageFetchRequestBuilder
{
    private PackageFetchRequest _request { get; set; } = new PackageFetchRequest();

    public static PackageFetchRequestBuilder Create() => new();

    public static PackageFetchRequestBuilder Create(PackageFetchRequest request)
    {
        return new PackageFetchRequestBuilder().WithReleaseTag(request.ReleaseTag)
                                               .WithPackageName(request.PackageName)
                                               .DownloadFrom(request.DownloadFromPath)
                                               .DownloadTo(request.DownloadToPath);
    }

    public PackageFetchRequestBuilder WithReleaseTag(string releaseTag)
    {
        _request.ReleaseTag = releaseTag;
        return this;
    }

    public PackageFetchRequestBuilder WithPackageName(string packageName)
    {
        _request.PackageName = packageName;
        return this;
    }

    public PackageFetchRequestBuilder DownloadFrom(string path)
    {
        _request.DownloadFromPath = path;
        return this;
    }

    public PackageFetchRequestBuilder DownloadTo(string path)
    {
        _request.DownloadToPath = path;
        return this;
    }

    public async Task<Result> ExecuteAsync(CancellationToken cancellationToken)
    {

        if (!Directory.Exists(_request.DownloadToPath))
            Directory.CreateDirectory(_request.DownloadToPath);

        var http = new HttpClient();

        var response = await http.GetAsync(_request.DownloadFromPath, cancellationToken);
        await using var fs = File.Create(_request.DownloadToPath);
        await response.Content.CopyToAsync(fs);

        return new Result(true, "Package downloaded successfully.");
    }
}