"""
A module that runs uncrustify on the workspace directory
"""

load("@bazel_skylib//lib:shell.bzl", "shell")

def _uncrustify_attr_factory():
    """
    Helper macro to generate a struct of attrs for use in a rule() definition.

    Returns:
      A dictionary of attributes relevant to the rule
    """
    attrs = {
        "uncrustify": attr.label(
            default = "@com_github_uncrustify_uncrustify//:uncrustify",
            cfg = "exec",
            executable = True,
        ),
        "config": attr.label(
            default = "//bazel/cpp/uncrustify:defaults.cfg",
            doc = "Uncrustify config file",
            allow_single_file = True,
        ),
        "log_level": attr.int(
            default = 0,
            doc = "Specify the log severity level",
        ),
        "use_backup": attr.bool(
            default = False,
            doc = "Set to True to create a backup for each file being modified",
        ),
        "check": attr.bool(
            default = False,
            doc = "Do not output the new text, instead verify that nothing changes when the file(s) are processed",
        ),
        "srcs": attr.label_list(
            allow_files = [
                ".cc",
                ".cpp",
                ".h",
                ".hpp",
            ],
            doc = "A list of labels representing the cc/cpp and h/hpp files to include in the test",
        ),
        "_runner": attr.label(
            default = "//bazel/cpp/uncrustify:runner.bash.template",
            allow_single_file = True,
        ),
    }

    return attrs

def _uncrustify_impl_factory(ctx):
    """
    Helper macro to generate a uncrustify rule.

    This macro does not depend on defaults encoded in the binary, instead
    preferring to set explicit values for each flag.

    Args:
      ctx:          The execution context.

    Returns:
      A DefaultInfo provider
    """

    args = [
        "-L %s" % ctx.attr.log_level,
        "-c %s" % ctx.file.config.path,
    ]

    if ctx.attr.check:
        args.append("--check")
    elif ctx.attr.use_backup:
        args.append("--replace")
    else:
        args.append("--no-backup")

    out_file = ctx.actions.declare_file(ctx.label.name + ".bash")
    substitutions = {
        "@@ARGS@@": shell.array_literal(args),
        "@@UNCRUSTIFY_SHORT_PATH@@": shell.quote(ctx.executable.uncrustify.short_path),
    }
    ctx.actions.expand_template(
        template = ctx.file._runner,
        output = out_file,
        substitutions = substitutions,
        is_executable = True,
    )

    runfiles = [ctx.executable.uncrustify] + ctx.files.srcs
    return DefaultInfo(
        files = depset([out_file]),
        runfiles = ctx.runfiles(files = runfiles),
        executable = out_file,
    )

def _uncrustify_test_impl(ctx):
    return [_uncrustify_impl_factory(ctx)]

_uncrustify_test = rule(
    implementation = _uncrustify_test_impl,
    attrs = _uncrustify_attr_factory(),
    test = True,
)

def uncrustify_test(**kwargs):
    """
    Wrapper for the _uncrustify_test rule. Disables sandboxing and caching.

    Args:
      **kwargs: all parameters for _uncrustify_test
    """
    tags = kwargs.get("tags", [])

    # Note: the "external" tag is a workaround for
    # https://github.com/bazelbuild/bazel/issues/15516.
    for t in ["no-sandbox", "no-cache", "external"]:
        if t not in tags:
            tags.append(t)
    kwargs["tags"] = tags
    _uncrustify_test(**kwargs)
