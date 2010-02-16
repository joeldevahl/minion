function build(settings)	
	local src = Collect("src/*.c")
	local objs = Compile(settings, src)
	local exe = Link(settings, "minion", objs)
	
	return exe
end

settings = NewSettings()
settings.config_name = "debug"
settings.cc.includes:Add("include")
settings.debug = 1
settings.optimize = 0

DefaultTarget(build(settings))
