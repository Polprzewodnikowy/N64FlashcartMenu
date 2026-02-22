## MP3 Player
<!-- Needs a screenshot of the MP3 player -->
The N64FlashcartMenu includes an MP3 Player that can read MP3 files from the SD Card. MP3 files should have a sample rate of 44100 Hz or higher.

Select an MP3 file in the File Browser to open the MP3 Player. Playback begins immediately.

### Controls

| Button | Action |
|--------|--------|
| `A` | Play / Pause |
| `B` | Stop and return to File Browser |
| `C-Up` | Previous track |
| `C-Down` | Next track |
| `C-Left` | Seek backward |
| `C-Right` | Seek forward |
| `D-Up` | Previous track |
| `D-Down` | Next track |
| `D-Left` | Seek backward |
| `D-Right` | Seek forward |

### Metadata

If the MP3 file contains ID3v2 tags, the player displays the track title, artist, album, and track number. Bitrate and sample rate are shown below the seek bar.

### Cover Art

The player will display cover art if available. It checks in the following order:

1. Embedded image in the MP3 file's ID3 APIC tag
2. The first `.jpg`, `.jpeg`, or `.png` file found in the same directory (alphabetical order)

Any image filename works — `cover.jpg`, `folder.png`, etc.

### Auto-advance

When a track finishes, the player automatically advances to the next MP3 file in the directory.
