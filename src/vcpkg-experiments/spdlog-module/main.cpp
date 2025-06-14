import spdlogmodule;

int main()
{
    // Still doesn't work, no workarounds available.
    // https://github.com/gabime/spdlog/issues/2701
    // https://developercommunity.visualstudio.com/t/Visual-Studio-cant-find-time-function/1126857
    // Attempted workaround with a shim for to12h, but this still didn't work.
    Log::Info("This is info {}", 1);
    Log::Warn({}, "This is warning {}", 1);
    Log::Error({}, "This is error {}", 1);

    Random2::FMT("a {}", 2);

    return 0;
}
