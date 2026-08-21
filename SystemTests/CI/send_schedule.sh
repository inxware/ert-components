
# Consider using the devport version of this instead? 
curl --header "Content-Type: application/json" --data '{"deviceId":"'$1'","playlist":"_data/'$2'"}' http://server/api/v1/content/sendplaylist.php
