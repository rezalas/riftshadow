#include "persisted_enum.h"
#include "mud.h"

//
// Says what was read and where, and says nothing about what will happen to it,
// because nothing happens to it. The value is kept and written back as it was
// found. This exists so that a file carrying a value the code has no name for
// is visible rather than silent, which is the state it was in before.
//
void report_unnamed_persisted(const char *family, const char *field, int raw)
{
	RS.Logger.Warn("{} {} has no name for the stored value {}, keeping it as it was read.", family, field, raw);
}
