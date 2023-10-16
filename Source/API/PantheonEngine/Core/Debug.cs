using System;
using System.Text;

using System.Runtime.InteropServices;

namespace PantheonEngine
{
    public static class Debug
    {
        public static void SetLogFile(string path)
        {
            Internal_SetFile(new StringBuilder(path));
        }

        public static void DisableLogFile()
        {
            Internal_SetFile(new StringBuilder());
        }

        public static void Log(string message)
        {
            Internal_Print(new StringBuilder(message), false);
        }

        public static void LogWarning(string message)
        {
            Internal_Print(new StringBuilder("[WARNING] ").Append(message), false);
        }

        public static void LogError(string message)
        {
            StringBuilder builder = new StringBuilder("[ERROR] ").Append(message);

#if PANTHEON_EDITOR || DEBUG
            builder.Append('\n').Append(new StackTrace(1, true));
#endif

            Internal_Print(builder, true);
        }

        public static void LogException(Exception exception)
        {
            StringBuilder builder = new StringBuilder("[ERROR] ").Append(exception.GetType().FullName);

            if (!string.IsNullOrWhiteSpace(exception.Message))
                builder.Append(": ").Append(exception.Message);

#if PANTHEON_EDITOR || DEBUG
            if (!string.IsNullOrWhiteSpace(exception.StackTrace))
                builder.Append('\n').Append(exception.StackTrace);
#endif

            Internal_Print(builder, true);
        }

        #region BINDINGS

        [DllImport(EngineLibs.PantheonCore, CallingConvention = CallingConvention.Cdecl, EntryPoint = "Logger_print")]
        private static extern void Internal_Print(StringBuilder message, bool isError);

        [DllImport(EngineLibs.PantheonCore, CallingConvention = CallingConvention.Cdecl, EntryPoint = "Logger_setFile")]
        private static extern void Internal_SetFile(StringBuilder filePath);

        #endregion
    }
}