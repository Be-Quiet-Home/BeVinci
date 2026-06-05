#ifndef BEVINCI_ADDON_EXPORT_H
#define BEVINCI_ADDON_EXPORT_H

/*
 * BeVinci add-ons are built for Haiku shared objects.
 *
 * GCC on Haiku exports the C factory symbol without a Windows-style
 * Windows-style export annotation. Keeping this macro explicit makes the
 * add-on boundary visible without carrying a foreign compiler attribute.
 */
#define BEVINCI_ADDON_EXPORT

#endif /* BEVINCI_ADDON_EXPORT_H */
