# Simulator
	simulation time: 12h
	max people simulated: 267785

# AquaPark
	Hours: 8h – 20h
		People come and go as they please
	maximum capacity: 10 000 people
		At same time
	Queue: 10

# Attractions:
## Swimming pool
	Duration: variable
	Ride: Max(100) at time
		starts: always running
	Queue: 100
		Priority: None
		Age: Free
## Toboggan
	Duration: 25 seconds
	Ride: 2 || 4  at time
		Starts: when 2 or 4 people are ready && 25 after last time
	Queue: 100
		Priority: Over 75 years
## Toboggan Race
	Duration: 10 - 15 seconds
	Ride: 1 - 4
		starts: every 20sec
	Queue: 100
		Priority: Over 75 years
## Sunbath
	Duration: variable
	Ride: Max(10 000)  at time
		starts: always running
	Queue: Instant access
# Person:
	Age: 0 - 122
	time on park: 0 - 12h
	maximum waiting time: 0 - 600 seconds

# No Rules:
