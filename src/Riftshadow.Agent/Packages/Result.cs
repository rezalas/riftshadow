namespace Riftshadow.Agent;

/// <summary>
/// A super simple result object. Indicates success of a process and 
/// has a message.
/// </summary>
/// <param name="Success">Whether or not this result indicates success.</param>
/// <param name="Message">The message with more details about the result.s</param>
public record Result(bool Success, string Message);

public record Result<T>(string Message, T Value) : Result(Value is not null, Message);