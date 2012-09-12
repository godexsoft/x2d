cd $1/game_resources
rm -f ../assets/resources.zip
zip -r -0 ../assets/resources.zip * -x "*/*.svn/*"
