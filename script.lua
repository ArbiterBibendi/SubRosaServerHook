function init()
	print("LUA SCRIPT INIT")
	time = os.time()
	gameTimer = 0
end

function loop()
	timeOld = time
	time = os.time()
	gameTimer = gameTimer + (time - timeOld)
	
	if gameTimer == 1 then
		gameTimer = 0
		spawnVehicle(12, 2951.29, 24.869, 1532.15, 1.0, 1.0, 1.0, 1)
	end
end
