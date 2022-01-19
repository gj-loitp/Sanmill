/*
  This file is part of Sanmill.
  Copyright (C) 2019-2021 The Sanmill developers (see AUTHORS file)

  Sanmill is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Sanmill is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

import 'package:flutter_test/flutter_test.dart';
import 'package:sanmill/models/display.dart';
import 'package:sanmill/services/mill/mill.dart';
import 'package:sanmill/services/storage/storage.dart';

import '../helpers/mocks/audios_mock.dart';
import '../helpers/mocks/storage_mock.dart';
import '../helpers/test_mills.dart';

void main() {
  group("Import Export Service", () {
    test(
        "import standard notation should populate the recorder with the imported moves",
        () async {
      const testMill = WinLessThanThreeGame();

      // initialize the test
      DB.instance = MockedDB();
      Audios.instance = MockedAudios();
      final controller = MillController();
      controller.gameInstance.gameMode = GameMode.humanVsHuman;

      // import a game
      ImportService.import(testMill.export);

      expect(MillController().recorder.toString(), testMill.recorder);
    });

    test("export standard notation", () async {
      const testMill = WinLessThanThreeGame();

      // initialize the test
      DB.instance = MockedDB();
      Audios.instance = MockedAudios();
      final controller = MillController();
      controller.gameInstance.gameMode = GameMode.humanVsHuman;

      // import a game
      ImportService.import(testMill.export);

      expect(controller.recorder.moveHistoryText, testMill.export);
    });

    test(
      "export not standard notation",
      () async {
        const testMill = WinLessThanThreeGame();

        // initialize the test
        final mockedDB = MockedDB();
        // ignore: deprecated_member_use_from_same_package
        mockedDB.display = const Display(standardNotationEnabled: false);
        DB.instance = mockedDB;
        Audios.instance = MockedAudios();
        final controller = MillController();
        controller.gameInstance.gameMode = GameMode.humanVsHuman;

        // import a game
        ImportService.import(testMill.export);

        expect(controller.recorder.moveHistoryText, testMill.exportNotStandard);
      },
      skip: "deprecated test. See [Display.standardNotationEnabled].",
    );
  });
}
