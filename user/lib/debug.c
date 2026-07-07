/*
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * |  S a c t i o n O S                                                         |
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * debug -- Temporary DebugPrint API, calls ScDebugPrint internally.
 *
 * Will be removed once the console subsystem and WriteFile(CON:, ...)
 * replace it.  Do not build new features on top of this.
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 */

#include "saction_api.h"

extern void ScDebugPrint(const char* msg);

void DebugPrint(const char* msg)
{
    ScDebugPrint(msg);
}
