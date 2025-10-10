namespace Riftshadow.Agent;

public record Result(bool Success, string Message);

public record Result<T>(bool Success, string Message, T Data) : Result(Success, Message);