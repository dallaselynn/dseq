/^# Packages using this file: / {
  s/# Packages using this file://
  ta
  :a
  s/ dseq / dseq /
  tb
  s/ $/ dseq /
  :b
  s/^/# Packages using this file:/
}
