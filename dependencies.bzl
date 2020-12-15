def generate_dependencies(target, dependencies, name):

    for index, dependency in enumerate(dependencies):
        rule_name = dependency.replace(".", "_")

        native.genrule(
            name = rule_name,
            srcs = [
                target
            ],
            cmd = "\n".join([
                "LOCATIONS=\"$(locations %s)\"" % target,
                "IFS=' ' read -ra LOCATIONS <<< $$LOCATIONS",

                # Search for our copy target
                "TARGET_BASENAME=$$(basename \"$@\")",
                "for LOCATION in $${LOCATIONS[@]}; do",
                    "LOCATION_BASENAME=$$(basename $$LOCATION)",

                    "if [ \"$$LOCATION_BASENAME\" == \"$$TARGET_BASENAME\" ]; then",
                        "cp $$LOCATION \"$@\"",
                        "exit 0",
                    "fi",
                "done",

                "echo !!! COULD NOT FIND $@",
                "exit 1"
            ]),
            outs = [
                dependency
            ]
        )

    native.filegroup(
        name = name,
        srcs = dependencies
    )
