Magnetic Imaging Tile
======

![SparkFun Magnetic Imaging Tile](https://cdn.sparkfun.com//assets/parts/1/2/8/3/9/Magnetic-Imager-Fan.gif)

[*SparkFun Magnetic Imaging Tile (SPX-14652)*](https://www.sparkfun.com/products/14652)

The Magnetic Imaging Tile uses an array of 64 hall effect sensors to convert magnetic fields to the visual spectrum. That's right! You can now see magnetic fields in real time! As is to be expected, there are caveats: the magnetic sensors used on the tile are some of the most sensitive on the market but you need to be within 1 to 2 centimeters of the tile to get a good image.

This is a board intended to function as a "magnetic field camera" to visualize magnetic fields. 

This is an endorsed fork and collaboration of [Peter Jansen's work](https://hackaday.io/project/18518-iteration-8/log/91551-a-third-high-speed-magnetic-imager-tile). All credit goes to him! SparkX has re-designed the PCB for DFM and simiplified some of the platform interfacing code.

### Version 3.0

The major advancement of v3.0 is a dramatic increase in the speed with which the tile data can be read out. ~2000 frames per second (fps) can be achieved which allows visualizing even quickly varying fields (e.g. those in a 60Hz transformer, or a moving motor). Version 3.0 reduces the size of the tile to an 8x8 grid of hall effect sensors (64 total), arrayed in a 4mm grid.  The boards is tile-able with up to 4 of the boards tileable with minimal borders to create a 16x16 array. 

For more information (and a video of the v3.0 tile in use), please see: 
https://hackaday.io/project/18518-iteration-8/log/91551-a-third-high-speed-magnetic-imager-tile-draft

Repository Contents
-------------------

* **/Documents** - Datasheets for the RV-1805-C3 and super capacitor
* **/Hardware** - Eagle design files (.brd, .sch)

License Information
-------------------

This product is _**open source**_! 

Please review the LICENSE.md file for license information. 

If you have any questions or concerns on licensing, please contact techsupport@sparkfun.com.

Please use, reuse, and modify these files as you see fit. Please maintain attribution to SparkFun Electronics and release any derivative under the same license.

Disclaimer
-------------------

**DISCLAIMER OF WARRANTIES AND LIMITATION OF LIABILITY.**

UNLESS OTHERWISE SEPARATELY UNDERTAKEN BY THE LICENSOR, TO THE EXTENT POSSIBLE, THE LICENSOR OFFERS THE LICENSED MATERIAL AS-IS AND AS-AVAILABLE, AND MAKES NO REPRESENTATIONS OR WARRANTIES OF ANY KIND CONCERNING THE LICENSED MATERIAL, WHETHER EXPRESS, IMPLIED, STATUTORY, OR OTHER. THIS INCLUDES, WITHOUT LIMITATION, WARRANTIES OF TITLE, MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, NON-INFRINGEMENT, ABSENCE OF LATENT OR OTHER DEFECTS, ACCURACY, OR THE PRESENCE OR ABSENCE OF ERRORS, WHETHER OR NOT KNOWN OR DISCOVERABLE. WHERE DISCLAIMERS OF WARRANTIES ARE NOT ALLOWED IN FULL OR IN PART, THIS DISCLAIMER MAY NOT APPLY TO YOU.

TO THE EXTENT POSSIBLE, IN NO EVENT WILL THE LICENSOR BE LIABLE TO YOU ON ANY LEGAL THEORY (INCLUDING, WITHOUT LIMITATION, NEGLIGENCE) OR OTHERWISE FOR ANY DIRECT, SPECIAL, INDIRECT, INCIDENTAL, CONSEQUENTIAL, PUNITIVE, EXEMPLARY, OR OTHER LOSSES, COSTS, EXPENSES, OR DAMAGES ARISING OUT OF THIS PUBLIC LICENSE OR USE OF THE LICENSED MATERIAL, EVEN IF THE LICENSOR HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH LOSSES, COSTS, EXPENSES, OR DAMAGES. WHERE A LIMITATION OF LIABILITY IS NOT ALLOWED IN FULL OR IN PART, THIS LIMITATION MAY NOT APPLY TO YOU.

THE DISCLAIMER OF WARRANTIES AND LIMITATION OF LIABILITY PROVIDED ABOVE SHALL BE INTERPRETED IN A MANNER THAT, TO THE EXTENT POSSIBLE, MOST CLOSELY APPROXIMATES AN ABSOLUTE DISCLAIMER AND WAIVER OF ALL LIABILITY.

