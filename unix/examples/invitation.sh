#!/bin/bash
guestfile=~/workspace/unix/examples/guests

function check_file {
	if [[ ! -e "$guestfile" ]]
	then
		printf "${guestfile##*/} doesn't exist\n"
		exit 1
	fi
}

check_file

export PLACE="Sarotini's"
(( Time = $(date +%H) + 1 ))
declare -a foods=(cheese crackers shrimp drinks '"hot dogs"' sandwhiches)
declare -i n=0

for person in $(cat $guestfile)
do
	if [[ $person == root ]]
	then
		continue
	else
		#Start of here document
		#mail -v -s "Party" $person <<- FINIS
		echo "Party" $person
		echo Hi $person! Please join me at $PLACE for a party!
		echo Meet me at $Time o\'clock.
		echo I\'ll bring the ice-cream. Would you please bring
		echo ${foods[$n]} and anything else you would like to eat?
		echo Let me know if you can make it.
		echo 	Hope to see you soon.
		echo 		Your pal,
		echo 		Sam@$(hostname)
		#FINIS
		n=n+1

		if (( ${#foods[*]} == $n ))
		then
			declare -a foods=(cheese, crackers, shrimp, drinks, '"hot dogs"' sandwhiches)
			n=0
		fi
	fi
done
printf "Bye...\n"

