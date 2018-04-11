# Roguelike Game [COM S 327] | File Parser
A parser for the plaintext config files that Dr. Shaeffer began using in version 1.07 of the semester-long project.

The parser requires LF line endings.

See `src/runner.cpp` or the included files in the `doc` folder for details on usage.

This can parse any file of a similar type to the one provided in samples, assuming the following:

1. Possess a header on the first line of the format "RLG327 [entity_type] [file_type] [version_number]"
2. Possess entities wrapped in BEGIN [ENTITY_NAME] ... END tags on their own lines.
3. Possess keys of at least two capital letters followed by a newline or a value.
  3a. If followed by a newline, the multiline values must end with a [.] on its own line.
