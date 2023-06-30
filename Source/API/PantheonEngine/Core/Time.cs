using System.Runtime.InteropServices;

namespace PantheonEngine
{
    public static class Time
    {
        public static float ElapsedTime
        {
            get => Internal_GetTime();
        }

        public static float UnscaledElapsedTime
        {
            get => Internal_GetUnscaledTime();
        }

        public static float DeltaTime
        {
            get => Internal_GetDeltaTime();
        }

        public static float UnscaledDeltaTime
        {
            get => Internal_GetUnscaledDeltaTime();
        }

        public static float TimeScale
        {
            get => Internal_GetTimeScale();
            set => Internal_SetTimeScale(value);
        }

        public static ulong FrameCount
        {
            get => Internal_GetFrameCount();
        }

        #region BINDINGS

        [DllImport(EngineLibs.PantheonCore, CallingConvention = CallingConvention.Cdecl, EntryPoint = "Timer_getTime")]
        private static extern float Internal_GetTime();

        [DllImport(EngineLibs.PantheonCore, CallingConvention = CallingConvention.Cdecl, EntryPoint = "Timer_getUnscaledTime")]
        private static extern float Internal_GetUnscaledTime();

        [DllImport(EngineLibs.PantheonCore, CallingConvention = CallingConvention.Cdecl, EntryPoint = "Timer_getDeltaTime")]
        private static extern float Internal_GetDeltaTime();

        [DllImport(EngineLibs.PantheonCore, CallingConvention = CallingConvention.Cdecl, EntryPoint = "Timer_getUnscaledDeltaTime")]
        private static extern float Internal_GetUnscaledDeltaTime();

        [DllImport(EngineLibs.PantheonCore, CallingConvention = CallingConvention.Cdecl, EntryPoint = "Timer_getFrameCount")]
        private static extern ulong Internal_GetFrameCount();

        [DllImport(EngineLibs.PantheonCore, CallingConvention = CallingConvention.Cdecl, EntryPoint = "Timer_getTimeScale")]
        private static extern float Internal_GetTimeScale();

        [DllImport(EngineLibs.PantheonCore, CallingConvention = CallingConvention.Cdecl, EntryPoint = "Timer_setTimeScale")]
        private static extern void Internal_SetTimeScale(float timeScale);

        #endregion
    }
}