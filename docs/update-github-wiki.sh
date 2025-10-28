#!/bin/bash
set -e
#Landing page furniture and ToCs etc.
cp ./docs/github-wiki-assets/* ../ert-components.wiki/

#SVGs and PNGS etc referenced in MD docs:
cp ./docs/illustrations/* ../ert-components.wiki/illustrations/

#Indiviual source README documents to export
cp ./docs/ert-porting-guide.md ../ert-components.wiki/
cp ./docs/ert-build-guide.md ../ert-components.wiki/

echo "Done"
