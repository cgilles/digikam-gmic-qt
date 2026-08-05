Under a Linux Ubuntu 22.04 or later:

Note: desired release tag must be the same for CImg and G'MIC. Look existing tags with command "git tag" in the respective repository.
To get the code at the desired tag, uses "git tag _version_" command.

* Checkout CImg in an external repository at the desired release tag (https://github.com/GreycLab/CImg.git).

* Checkout G'MIC in an external repository at the desired release tag (https://github.com/GreycLab/gmic).

* Checkout G'MIC-Qt code in an external repository at master branch (https://github.com/GreycLab/gmic-qt).

* From the external G'MIC-Qt repository, fix dependencies to complete the configuration of G'MIC-Qt source code.
  Use the _bootstrap-none_ script to configure G'MIC-Qt directory with the "none" host.
  Note: The G'MIC-Qt/translations and G'MIC-Qt/translations/filters must be build before. The don't need to be compiled.

* From this repository, remove the gmicqt/ directory with "git rm -fr gmicqt".

* When configuration is done, in this repository:
    - Copy the external G'MIC-Qt contents to the gmicqt/.
    - Copy the G'MIC external directory contents to the gmicqt/gmic/.
    - Copy the CImg.h file from the external CImg directory to the gmicqt/gmic/src/.

* In this reprository, remove the .gitignore and .git files from gmicqt/gmic/ and gmicqt/.

* Go to the root of this repository and add gmicqt/ directory to git with the command "git add gmicqt".

* Patch gmicqt/CMakeLists.txt to add the "digikam" host rules and to use local gmic dir:

```
patch -p1 < ./src/patches/01_digikam_root_cmakelists.patch
```

* Patch gmicqt/ codes to disable theming (redondant with digiKam one and crash host):

```
patch -p1 < ./src/patches/02_digikam_disable_theming.patch
```

* Patch gmicqt/ codes to disable HDPI settings (redondant with digiKam one):

```
patch -p1 < ./src/patches/03_digikam_disable_hdpi.patch
```

* Patch gmicqt/ codes to disable logo settings and optimize space (logo moves in settings dialog):

```
patch -p1 < ./src/patches/04_digikam_disable_logo.patch
```

* Patch gmicqt/ codes to improve plugin integration:

```
patch -p1 < ./src/patches/05_digikam_plugin_integration.patch
```

* Patch gmicqt/ codes to fix crash when Cancel button is pressed while computing:

```
patch -p1 < ./src/patches/06_digikam_fix_cancel_crash.patch
```

* Patch gmicqt/ codes to fix crash when filters tree-view context menu is activated:

```
patch -p1 < ./src/patches/07_digikam_fix_crash_495810.patch
```

* Patch gmicqt/ codes to change lead dialog from QMainWindow to QWidget to be included in QWizad as page:

```
patch -p1 < ./src/patches/08_digikam_qmainwindow_to_qwidget.patch
```

* Check if new files need to be appended with "git status". Add files to git repository if necessary.

* Check compilation with "digikam" host eg. using the bootstrap.linux.

