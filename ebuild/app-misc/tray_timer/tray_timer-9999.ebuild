# Copyright 1999-2015 Gentoo Foundation
# Distributed under the terms of the GNU General Public License v2
# $Header: $

EAPI=5

inherit git-r3
inherit cmake-utils

DESCRIPTION="Tiny timer applet which sit on system tray."
HOMEPAGE="https://github.com/stream009/tray_timer"

EGIT_REPO_URI="git://github.com/stream009/tray_timer.git"

LICENSE="GPL"
SLOT="0"
KEYWORDS="~x86"
IUSE=""

DEPEND="
	>=dev-util/cmake-3.3.0
	dev-qt/qtgui:4"
RDEPEND="${DEPEND}"

S="${S}/src"
