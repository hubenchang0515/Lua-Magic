if #arg ~= 1 then
	io.stderr:write(string.format("Usage : %s <file>", arg[0]))
	os.exit(1);
end

input = io.open(arg[1])
if input == nil then
	io.stderr:write(string.format("Open input file '%s'  failed", arg[0]))
	os.exit(1)
end

file = arg[1] .. ".cpp"
output = io.open(file,"w")
if output == nil then
	io.stderr:write(string.format("Create output file '%s'  failed", file))
	os.exit(1)
end

output:write("int " .. arg[1] .. "_bind(lua_State* L)\n{\n")
while true do
	fun = input:read("l")
	if fun == nil then
		break
	end
	output:write(string.format('\tluaMagic::bind(L, "%s", %s );\n', fun, fun));
end
output:write("}\n")