namespace PantheonEngine
{
    internal static class EngineLibs
    {
        private const string Directory = "engine/";

#if DEBUG
        internal const string PantheonCore = Directory + "PantheonCored";
        internal const string PantheonAudio = Directory + "PantheonAudiod";
        internal const string PantheonPhysics = Directory + "PantheonPhysicsd";
        internal const string PantheonRendering = Directory + "PantheonRenderingd";
        internal const string PantheonUI = Directory + "PantheonUId";
        internal const string PantheonApp = Directory + "PantheonAppd";
#else
        internal const string PantheonCore = Directory + "PantheonCore";
        internal const string PantheonAudio = Directory + "PantheonAudio";
        internal const string PantheonPhysics = Directory + "PantheonPhysics";
        internal const string PantheonRendering = Directory + "PantheonRendering";
        internal const string PantheonUI = Directory + "PantheonUI";
        internal const string PantheonApp = Directory + "PantheonApp";
#endif
    }
}