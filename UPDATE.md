Under a Linux Ubuntu 22.04 or later:

Note: desired release tag must be the same for CImg, G'MIC, and G'MIC-Qt.

* Checkout CImg at the desired release tag (https://github.com/GreycLab/CImg.git).

* Checkout G'MIC at the desired release tag (https://github.com/GreycLab/gmic).

* Checkout G'MIC-Qt at the same desired release tag (https://github.com/GreycLab/gmic-qt).

* Fix dependencies to complete the configuration of G'MIC-Qt source code.
  Use the _bootstrap-none_ script to configure G'MIC-Qt directory with the "none" host.
  Note: The G'MIC-Qt/translations and G'MIC-Qt/translations/filters must be build before.

* Remove the gmicqt/ directory.

* When configuration is done copy the G'MIC directory contents to the gmicqt/gmic/.

* Copy the G'MIC-Qt contents to the gmicqt/.

* Remove the .gitignore files from gmicqt/gmic/ and gmicqt/.

* Add gmicqt/ directory to git.

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

* Patch gmicqt/ codes to fix macOS targets:

```
patch -p1 < ./src/patches/07_digikam_macos_target.patch
```

* Patch gmicqt/ codes to fix crash when filters tree-view context menu is actived:

```
patch -p1 < ./src/patches/08_digikam_fix_crash_495810.patch
```

* Patch gmicqt/ codes to change lead dialog from QMainWindow to QWidget to be included in QWizad as page:

```
patch -p1 < ./src/patches/09_digikam_qmainwindow_to_qwidget.patch
```

* Check if new files need to be appended with "git status". Add files to git repository if necessary.

* Check compilation with "digikam" host.

