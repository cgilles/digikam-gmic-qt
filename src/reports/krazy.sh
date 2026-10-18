#!/bin/bash

# SPDX-FileCopyrightText: 2013-2026 by Gilles Caulier, <caulier dot gilles at gmail dot com>
#
# Run Krazy static analyzer on whole digiKam source code.
# https://github.com/Krazy-collection/krazy
#
# Production revision: master
#
# SPDX-License-Identifier: BSD-3-Clause
#

# Halt and catch errors
set -eE
trap 'PREVIOUS_COMMAND=$THIS_COMMAND; THIS_COMMAND=$BASH_COMMAND' DEBUG
trap 'echo "FAILED COMMAND: $PREVIOUS_COMMAND"' ERR

. ./common.sh

StartScript
checksCPUCores

# Check run-time dependencies

if [ ! -f /opt/krazy/bin/krazy2all ] ; then

    echo "Krazy Static analyzer is not installed in /opt/krazy."
    echo "Please install Krazy from https://github.com/Krazy-collection/krazy"
    echo "Aborted..."
    exit 1

else

    echo "Check Krazy static analyzer passed..."

fi

export PATH=$PATH:/opt/krazy/bin

ORIG_WD="`pwd`"
REPORT_DIR="report.krazy"
KRAZY_VERSION=$(krazy2all --version)

# Source code root dir (resolved as absolu path)

TOP_DIR="$(realpath ../../)"

# Get active git branches to create report description string

TITLE="Gmic-Qt-$(parseGitBranch)$(parseGitHash)"
echo "Krazy Static Analyzer task name: $TITLE"
echo "Output Directory               : $REPORT_DIR"
echo "Krazy version                  : $KRAZY_VERSION"

rm -fr $ORIG_WD/$REPORT_DIR

# Compute static analyzer output as XML

krazy2all --export textedit \
          --title $TITLE \
          --no-brief \
          --strict all \
          --priority all \
          --explain \
          --check-set kde,foss \
          --extra null,contractions,defines,qmacros \
          --exclude qclasses,license \
          --exclude-types qml,python,qdoc,perl \
          --config .krazy \
          --topdir ../ \
          --outfile ./report.krazy.txt \
          > ./krazy-trace.txt 2>&1

ret=$?

mkdir -p $ORIG_WD/$REPORT_DIR

if [[ $ret == "0" ]]; then

    mv ./krazy-trace.txt $REPORT_DIR

    # Create an HTML report

    REPORT_FILE="./report.krazy.txt"
    HTML_FILE="index.html"

    # HTML header

    cat > "$HTML_FILE" << 'EOF'
        <!DOCTYPE html>
        <html lang="en">
        <head>
            <meta charset="UTF-8">
            <meta name="viewport" content="width=device-width, initial-scale=1.0">
            <title>Krazy Report</title>
            <style>
                :root {
                    --bg: #1e1e1e;
                    --fg: #d4d4d4;
                    --border: #3c3c3c;
                    --th-bg: #2d2d2d;
                    --row-even: #252525;
                }
                @media (prefers-color-scheme: light) {
                    :root {
                        --bg: #ffffff;
                        --fg: #333333;
                        --border: #dddddd;
                        --th-bg: #f2f2f2;
                        --row-even: #f9f9f9;
                    }
                }
                body { font-family: Arial, sans-serif; margin: 20px; background-color: var(--bg); color: var(--fg); }
                h1 { color: var(--fg); }
                table { border-collapse: collapse; width: 100%; margin-bottom: 20px; }
                th, td { border: 1px solid var(--border); padding: 8px; text-align: left; }
                th { background-color: var(--th-bg); }
                tr:nth-child(even) { background-color: var(--row-even); }
                .error { color: #ff6b6b; font-weight: bold; }
                .file { color: #6cb6ff; }
                .line { color: #7fd17f; }
                a { color: #6cb6ff; }
            </style>
        </head>
        <body>
        <h1>Krazy Report for __TITLE__</h1>
        <p>Total Errors: __ERRORS__</p>
        <table>
            <thead>
                <tr>
                    <th>File</th>
                    <th>Line</th>
                    <th>Error</th>
                    <th>Description</th>
                </tr>
            </thead>
        <tbody>
EOF

    # Read report file and generate the spreadsheet HTML

    total_errors=0

    while IFS= read -r line ; do

        total_errors=$((total_errors+1))

        # Uses awk to extract all info

        file=$(echo "$line" | awk -F':' '{print $1}')
        line_num=$(echo "$line" | awk -F':' '{print $2}')
        error=$(echo "$line" | awk -F':' '{print $3}')
        description=$(echo "$line" | awk -F':' '{print $NF}')

        # Cleanup file path to make it relative to the repository root

        clean_file="${file#$TOP_DIR/}"
        clean_file="${clean_file#../../}"

        # ESC special char for the HTML code

        file_escaped=$(echo "$clean_file" | sed 's/&/\&amp;/g; s/</\&lt;/g; s/>/\&gt;/g; s/"/\&quot;/g; s/'\''/\&#39;/g;')
        error_escaped=$(echo "$error" | sed 's/&/\&amp;/g; s/</\&lt;/g; s/>/\&gt;/g; s/"/\&quot;/g; s/'\''/\&#39;/g;')
        description_escaped=$(echo "$description" | sed 's/&/\&amp;/g; s/</\&lt;/g; s/>/\&gt;/g; s/"/\&quot;/g; s/'\''/\&#39;/g;')

        # Generate the URL for the Git repository

        git_url="https://invent.kde.org/graphics/digikam/-/tree/master/$clean_file?ref_type=heads#L$line_num"

        # Append a line to the spreadsheet

        cat >> "$HTML_FILE" << EOF
            <tr>
                <td class="file">$file_escaped</td>
                <td class="line"><a href="$git_url" target="_blank">$line_num</a></td>
                <td class="error">$error_escaped</td>
                <td>$description_escaped</td>
            </tr>
EOF

    done < "$REPORT_FILE"

    date=$(date +"%Y-%m-%d")

    # HTML footer

    cat >> "$HTML_FILE" << 'EOF'
        </tbody>
        </table>
            <p>Report generated by <a href="https://github.com/Krazy-collection/krazy">Krazy static analyzer </a> version __VERSION__ at __DATE__</p>
        </body>
    </html>
EOF

    sed -i "s|__TITLE__|$TITLE|g" $HTML_FILE
    sed -i "s|__ERRORS__|$total_errors|g" $HTML_FILE
    sed -i "s|__DATE__|$date|g" $HTML_FILE
    sed -i "s|__VERSION__|$KRAZY_VERSION|g" $HTML_FILE
    mv ./$HTML_FILE $REPORT_DIR

    rm -f ./report.krazy.txt

fi

cd $ORIG_WD

TerminateScript
