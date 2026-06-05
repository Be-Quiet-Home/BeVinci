# Upstream

BeVinci is a respectful hard fork of Medo by Zenja Solaja.

Original upstream:
https://github.com/smallstepforman/Medo

BeVinci starts from the Medo R1.0.0-Beta1-5.1 release line, plus the minimal
FreeType build fix required for the current Haiku R1/beta5 development setup.

The newer upstream `main` branch is kept as a reference through the `upstream`
remote, but it is not used as the BeVinci foundation because it currently moves
the ffmpeg export path beyond the stable HaikuPorts baseline targeted by this
fork.

BeVinci's goal is not to erase Medo's history. Medo is the original Haiku-native
media editor foundation. BeVinci continues that work with a focus on source
hygiene, documented build requirements, stable Haiku compatibility, hardening,
and workstation-grade video editing capabilities.
