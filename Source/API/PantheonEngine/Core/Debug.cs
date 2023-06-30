using System;
using System.Diagnostics;
using System.Text;

#if PANTHEON_EDITOR || DEBUG
using System.Runtime.InteropServices;
#endif

namespace PantheonEngine
{
    public static class Debug
    {
        public static void SetLogFile(string path)
        {
            Internal_OpenFile(new StringBuilder(path));
        }

        public static void DisableLogFile()
        {
            Internal_CloseFile();
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

        [DllImport(EngineLibs.PantheonCore, CallingConvention = CallingConvention.Cdecl, EntryPoint = "Logger_openFile")]
        private static extern void Internal_OpenFile(StringBuilder filePath);

        [DllImport(EngineLibs.PantheonCore, CallingConvention = CallingConvention.Cdecl, EntryPoint = "Logger_closeFile")]
        private static extern void Internal_CloseFile();

        #endregion
    }
}