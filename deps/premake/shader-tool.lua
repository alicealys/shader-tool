shader_tool = {
    source = path.join("include")
}

function shader_tool.import()
    links {"shader-tool-include"}
    shader_tool.includes()
end

function shader_tool.includes()
    includedirs {
		path.join(shader_tool.source),
	}
end

function shader_tool.project()
    project "shader-tool-include"
		kind "StaticLib"
		language "C++"
        warnings "Off"

		shader_tool.includes()

        files {
			path.join(shader_tool.source, "shader-tool/**.hpp"),
			path.join(shader_tool.source, "shader-tool/**.cpp"),
		}
        
end

table.insert(dependencies, shader_tool)
