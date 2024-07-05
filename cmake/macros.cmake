macro(apply_project_defaults target)
    target_link_libraries(${target}
      PRIVATE
        project_features
        project_definitions
        project_options
        project_warnings
        project_errors
    )
endmacro()
