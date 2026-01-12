#include <stdio.h>
#include "motionErrorHandler.h"

// error_manager.c

#define MAX_OBSERVERS 3
static ErrorHandlerCallback_t observer_list[MAX_OBSERVERS] = {NULL};
static int observer_count = 0;

// Function to register a module to receive error notifications
int ErrorManager_RegisterObserver(ErrorHandlerCallback_t callback) {
    if (observer_count < MAX_OBSERVERS) {
        observer_list[observer_count++] = callback;
        return 0;
    }
    return -1; // Registration failed
}

// Function to notify all registered observers
static void ErrorManager_Notify(const Error_t* error) {
    printf("  [MANAGER]: Notifying %d registered handlers...\n", observer_count);
    for (int i = 0; i < observer_count; i++) {
        if (observer_list[i] != NULL) {
            observer_list[i](error);
        }
    }
    printf("  [MANAGER]: Notification complete.\n");
}

// Main entry point for the Master to report an error
void Master_ReportError(const Error_t* error) {
    printf("\n--- ERROR REPORTED ---\n");
    printf("  [MASTER]: Detected failure: %s\n", error->message);
    ErrorManager_Notify(error);
    printf("----------------------\n");
}


// observers.c


// --- OBSERVER 1: LOGGER ---
void Logger_Callback(const Error_t* err) {
    printf("    [LOGGER]: Writing to persistent log: Source=%d, Code=%d, Severity=%d\n",
           err->source, err->code, err->severity);
    // In a real system, this would write to Flash or a file.
}

// --- OBSERVER 2: FAILSAFE UNIT (Dynamic Solution) ---
void Failsafe_Callback(const Error_t* err) {
    if (err->severity >= SEVERITY_CRITICAL) {
        printf("    [FAILSAFE]: CRITICAL ACTION NEEDED! Source=%d\n", err->source);

        if (err->source == SOURCE_DRIVER_3 && err->code == ERR_TIMEOUT) {
            printf("    [FAILSAFE]: Executing dynamic solution: Attempting soft reset on Driver 3.\n");
            // Failsafe Unit changes the state of the system or component
        } else if (err->source == SOURCE_MASTER && err->severity == SEVERITY_FATAL) {
            printf("    [FAILSAFE]: FATAL ERROR. Initiating controlled system halt.\n");
            // set global flag for system shutdown
        }
    }
}

// --- OBSERVER 3: DISPLAY/HMI ---
void Display_Callback(const Error_t* err) {
    if (err->severity >= SEVERITY_WARNING) {
        printf("    [DISPLAY]: Updating HMI: Warning Level %d: %s\n", err->severity, err->message);
    }
}


// main.c

// Function to simulate a master task failing
void Master_PerformTask(void) {
    // 1. Simulate the Master trying to talk to Driver 3

    // 2. The underlying communication function returns an error structure
    Error_t driver_error = {
        .source = SOURCE_DRIVER_3,
        .code = ERR_TIMEOUT,
        .severity = SEVERITY_CRITICAL,
        .timestamp = 1000,
        .message = "Driver 3 did not respond within 500ms."
    };

    // 3. The Master's internal layer (or FSM) decides to report the error
    Master_ReportError(&driver_error);

    // --- Second Error Simulation ---
    Error_t master_internal_error = {
        .source = SOURCE_MASTER,
        .code = ERR_INVALID_CONFIG,
        .severity = SEVERITY_FATAL,
        .timestamp = 2000,
        .message = "Critical configuration data corrupted in RAM."
    };
    Master_ReportError(&master_internal_error);
}

int main() {
    printf("--- MASTER SYSTEM INITIALIZATION ---\n");

    // The Master registers all its error-handling modules during startup
    if (ErrorManager_RegisterObserver(Logger_Callback) == 0 &&
        ErrorManager_RegisterObserver(Failsafe_Callback) == 0 &&
        ErrorManager_RegisterObserver(Display_Callback) == 0) {
        printf("[INIT]: All 3 error observers successfully registered.\n");
    } else {
        printf("[INIT]: ERROR: Failed to register all observers.\n");
        return 1;
    }

    Master_PerformTask();

    return 0;
}



