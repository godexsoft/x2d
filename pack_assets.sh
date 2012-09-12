cd $1/game_resources
mkdir -p ../assets
rm -f ../assets/resources.zip
zip -r -0 ../assets/resources.zip * -x "*/*.svn/*"
