<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE eagle SYSTEM "eagle.dtd">
<eagle version="9.6.0">
<drawing>
<settings>
<setting alwaysvectorfont="no"/>
<setting verticaltext="up"/>
</settings>
<grid distance="0.1" unitdist="inch" unit="inch" style="lines" multiple="1" display="yes" altdistance="0.01" altunitdist="inch" altunit="inch"/>
<layers>
<layer number="1" name="Top" color="4" fill="1" visible="no" active="no"/>
<layer number="16" name="Bottom" color="1" fill="1" visible="no" active="no"/>
<layer number="17" name="Pads" color="2" fill="1" visible="no" active="no"/>
<layer number="18" name="Vias" color="2" fill="1" visible="no" active="no"/>
<layer number="19" name="Unrouted" color="6" fill="1" visible="no" active="no"/>
<layer number="20" name="Dimension" color="15" fill="1" visible="no" active="no"/>
<layer number="21" name="tPlace" color="7" fill="1" visible="no" active="no"/>
<layer number="22" name="bPlace" color="7" fill="1" visible="no" active="no"/>
<layer number="23" name="tOrigins" color="15" fill="1" visible="no" active="no"/>
<layer number="24" name="bOrigins" color="15" fill="1" visible="no" active="no"/>
<layer number="25" name="tNames" color="7" fill="1" visible="no" active="no"/>
<layer number="26" name="bNames" color="7" fill="1" visible="no" active="no"/>
<layer number="27" name="tValues" color="7" fill="1" visible="no" active="no"/>
<layer number="28" name="bValues" color="7" fill="1" visible="no" active="no"/>
<layer number="29" name="tStop" color="7" fill="3" visible="no" active="no"/>
<layer number="30" name="bStop" color="7" fill="6" visible="no" active="no"/>
<layer number="31" name="tCream" color="7" fill="4" visible="no" active="no"/>
<layer number="32" name="bCream" color="7" fill="5" visible="no" active="no"/>
<layer number="33" name="tFinish" color="6" fill="3" visible="no" active="no"/>
<layer number="34" name="bFinish" color="6" fill="6" visible="no" active="no"/>
<layer number="35" name="tGlue" color="7" fill="4" visible="no" active="no"/>
<layer number="36" name="bGlue" color="7" fill="5" visible="no" active="no"/>
<layer number="37" name="tTest" color="7" fill="1" visible="no" active="no"/>
<layer number="38" name="bTest" color="7" fill="1" visible="no" active="no"/>
<layer number="39" name="tKeepout" color="4" fill="11" visible="no" active="no"/>
<layer number="40" name="bKeepout" color="1" fill="11" visible="no" active="no"/>
<layer number="41" name="tRestrict" color="4" fill="10" visible="no" active="no"/>
<layer number="42" name="bRestrict" color="1" fill="10" visible="no" active="no"/>
<layer number="43" name="vRestrict" color="2" fill="10" visible="no" active="no"/>
<layer number="44" name="Drills" color="7" fill="1" visible="no" active="no"/>
<layer number="45" name="Holes" color="7" fill="1" visible="no" active="no"/>
<layer number="46" name="Milling" color="3" fill="1" visible="no" active="no"/>
<layer number="47" name="Measures" color="7" fill="1" visible="no" active="no"/>
<layer number="48" name="Document" color="7" fill="1" visible="no" active="no"/>
<layer number="49" name="Reference" color="7" fill="1" visible="no" active="no"/>
<layer number="51" name="tDocu" color="7" fill="1" visible="no" active="no"/>
<layer number="52" name="bDocu" color="7" fill="1" visible="no" active="no"/>
<layer number="88" name="SimResults" color="9" fill="1" visible="yes" active="yes"/>
<layer number="89" name="SimProbes" color="9" fill="1" visible="yes" active="yes"/>
<layer number="90" name="Modules" color="5" fill="1" visible="yes" active="yes"/>
<layer number="91" name="Nets" color="2" fill="1" visible="yes" active="yes"/>
<layer number="92" name="Busses" color="1" fill="1" visible="yes" active="yes"/>
<layer number="93" name="Pins" color="2" fill="1" visible="no" active="yes"/>
<layer number="94" name="Symbols" color="4" fill="1" visible="yes" active="yes"/>
<layer number="95" name="Names" color="7" fill="1" visible="no" active="yes"/>
<layer number="96" name="Values" color="7" fill="1" visible="yes" active="yes"/>
<layer number="97" name="Info" color="7" fill="1" visible="yes" active="yes"/>
<layer number="98" name="Guide" color="6" fill="1" visible="yes" active="yes"/>
</layers>
<schematic xreflabel="%F%N/%S.%C%R" xrefpart="/%S.%C%R">
<libraries>
<library name="special" urn="urn:adsk.eagle:library:367">
<description>&lt;b&gt;Special Devices&lt;/b&gt;&lt;p&gt;
7-segment displays, switches, heatsinks, crystals, transformers, etc.&lt;p&gt;
&lt;author&gt;Created by librarian@cadsoft.de&lt;/author&gt;</description>
<packages>
<package name="BATTERY" urn="urn:adsk.eagle:footprint:26592/1" library_version="3">
<description>&lt;B&gt;BATTERY&lt;/B&gt;&lt;p&gt;
22 mm</description>
<wire x1="0.635" y1="2.54" x2="0.635" y2="0" width="0.1524" layer="21"/>
<wire x1="-2.54" y1="0" x2="-0.635" y2="0" width="0.1524" layer="21"/>
<wire x1="0.635" y1="0" x2="2.54" y2="0" width="0.1524" layer="21"/>
<wire x1="0.635" y1="0" x2="0.635" y2="-2.54" width="0.1524" layer="21"/>
<wire x1="1.27" y1="-3.175" x2="2.54" y2="-3.175" width="0.1524" layer="21"/>
<wire x1="1.905" y1="-2.54" x2="1.905" y2="-3.81" width="0.1524" layer="21"/>
<circle x="0" y="0" radius="11.43" width="0.1524" layer="21"/>
<circle x="0" y="0" radius="10.2362" width="0.1524" layer="21"/>
<pad name="-" x="-5.715" y="0" drill="1.016" shape="long"/>
<pad name="+" x="9.525" y="-5.08" drill="1.016" shape="long"/>
<pad name="+@1" x="9.525" y="5.08" drill="1.016" shape="long"/>
<text x="-4.1656" y="6.35" size="1.778" layer="25" ratio="10">&gt;NAME</text>
<text x="-4.445" y="3.81" size="1.778" layer="27" ratio="10">&gt;VALUE</text>
<rectangle x1="-0.635" y1="-1.27" x2="0" y2="1.27" layer="21"/>
</package>
</packages>
<packages3d>
<package3d name="BATTERY" urn="urn:adsk.eagle:package:26670/1" type="box" library_version="3">
<description>BATTERY
22 mm</description>
<packageinstances>
<packageinstance name="BATTERY"/>
</packageinstances>
</package3d>
</packages3d>
<symbols>
<symbol name="BATTERY" urn="urn:adsk.eagle:symbol:26591/1" library_version="3">
<wire x1="-1.27" y1="3.81" x2="-1.27" y2="-3.81" width="0.4064" layer="94"/>
<wire x1="0" y1="1.27" x2="0" y2="-1.27" width="0.4064" layer="94"/>
<wire x1="1.27" y1="3.81" x2="1.27" y2="-3.81" width="0.4064" layer="94"/>
<wire x1="2.54" y1="1.27" x2="2.54" y2="-1.27" width="0.4064" layer="94"/>
<wire x1="-2.54" y1="0" x2="-1.524" y2="0" width="0.1524" layer="94"/>
<text x="-3.81" y="5.08" size="1.778" layer="95">&gt;NAME</text>
<text x="-3.81" y="-6.35" size="1.778" layer="96">&gt;VALUE</text>
<pin name="-" x="5.08" y="0" visible="off" length="short" direction="pwr" rot="R180"/>
<pin name="+" x="-5.08" y="0" visible="off" length="short" direction="pwr"/>
<pin name="+@1" x="-2.54" y="0" visible="off" length="short" direction="pwr" rot="R180"/>
</symbol>
</symbols>
<devicesets>
<deviceset name="BATTERY" urn="urn:adsk.eagle:component:26693/2" prefix="BAT" uservalue="yes" library_version="3">
<description>&lt;B&gt;BATTERY&lt;/B&gt;</description>
<gates>
<gate name="G$1" symbol="BATTERY" x="0" y="0"/>
</gates>
<devices>
<device name="" package="BATTERY">
<connects>
<connect gate="G$1" pin="+" pad="+"/>
<connect gate="G$1" pin="+@1" pad="+@1"/>
<connect gate="G$1" pin="-" pad="-"/>
</connects>
<package3dinstances>
<package3dinstance package3d_urn="urn:adsk.eagle:package:26670/1"/>
</package3dinstances>
<technologies>
<technology name="">
<attribute name="POPULARITY" value="3" constant="no"/>
</technology>
</technologies>
</device>
</devices>
</deviceset>
</devicesets>
</library>
<library name="supply1" urn="urn:adsk.eagle:library:371">
<description>&lt;b&gt;Supply Symbols&lt;/b&gt;&lt;p&gt;
 GND, VCC, 0V, +5V, -5V, etc.&lt;p&gt;
 Please keep in mind, that these devices are necessary for the
 automatic wiring of the supply signals.&lt;p&gt;
 The pin name defined in the symbol is identical to the net which is to be wired automatically.&lt;p&gt;
 In this library the device names are the same as the pin names of the symbols, therefore the correct signal names appear next to the supply symbols in the schematic.&lt;p&gt;
 &lt;author&gt;Created by librarian@cadsoft.de&lt;/author&gt;</description>
<packages>
</packages>
<symbols>
<symbol name="GND" urn="urn:adsk.eagle:symbol:26925/1" library_version="1">
<wire x1="-1.905" y1="0" x2="1.905" y2="0" width="0.254" layer="94"/>
<text x="-2.54" y="-2.54" size="1.778" layer="96">&gt;VALUE</text>
<pin name="GND" x="0" y="2.54" visible="off" length="short" direction="sup" rot="R270"/>
</symbol>
</symbols>
<devicesets>
<deviceset name="GND" urn="urn:adsk.eagle:component:26954/1" prefix="GND" library_version="1">
<description>&lt;b&gt;SUPPLY SYMBOL&lt;/b&gt;</description>
<gates>
<gate name="1" symbol="GND" x="0" y="0"/>
</gates>
<devices>
<device name="">
<technologies>
<technology name=""/>
</technologies>
</device>
</devices>
</deviceset>
</devicesets>
</library>
</libraries>
<attributes>
</attributes>
<variantdefs>
</variantdefs>
<classes>
<class number="0" name="default" width="0" drill="0">
</class>
</classes>
<parts>
<part name="BAT1" library="special" library_urn="urn:adsk.eagle:library:367" deviceset="BATTERY" device="" package3d_urn="urn:adsk.eagle:package:26670/1"/>
<part name="GND1" library="supply1" library_urn="urn:adsk.eagle:library:371" deviceset="GND" device=""/>
</parts>
<sheets>
<sheet>
<plain>
<wire x1="2.54" y1="78.74" x2="2.54" y2="43.18" width="0.3048" layer="94"/>
<wire x1="2.54" y1="43.18" x2="17.78" y2="43.18" width="0.3048" layer="94"/>
<wire x1="17.78" y1="43.18" x2="17.78" y2="78.74" width="0.3048" layer="94"/>
<wire x1="17.78" y1="78.74" x2="2.54" y2="78.74" width="0.3048" layer="94"/>
<wire x1="63.5" y1="78.74" x2="63.5" y2="20.32" width="0.3048" layer="94"/>
<wire x1="63.5" y1="20.32" x2="88.9" y2="20.32" width="0.3048" layer="94"/>
<wire x1="88.9" y1="20.32" x2="88.9" y2="78.74" width="0.3048" layer="94"/>
<wire x1="88.9" y1="78.74" x2="81.28" y2="78.74" width="0.3048" layer="94"/>
<wire x1="81.28" y1="78.74" x2="71.12" y2="78.74" width="0.3048" layer="94"/>
<wire x1="71.12" y1="78.74" x2="63.5" y2="78.74" width="0.3048" layer="94"/>
<wire x1="35.56" y1="43.18" x2="35.56" y2="22.86" width="0.3048" layer="94"/>
<wire x1="35.56" y1="22.86" x2="50.8" y2="22.86" width="0.3048" layer="94"/>
<wire x1="50.8" y1="22.86" x2="50.8" y2="43.18" width="0.3048" layer="94"/>
<wire x1="50.8" y1="43.18" x2="35.56" y2="43.18" width="0.3048" layer="94"/>
<wire x1="71.12" y1="78.74" x2="71.12" y2="73.66" width="0.3048" layer="94"/>
<wire x1="71.12" y1="73.66" x2="81.28" y2="73.66" width="0.3048" layer="94"/>
<wire x1="81.28" y1="73.66" x2="81.28" y2="78.74" width="0.3048" layer="94"/>
<text x="40.386" y="24.13" size="1.778" layer="94">GND</text>
<text x="73.914" y="21.336" size="1.778" layer="94">GND</text>
<text x="44.196" y="36.83" size="1.778" layer="94">3.3V</text>
<text x="65.024" y="37.338" size="1.778" layer="94">3V3</text>
<text x="30.48" y="60.96" size="3.81" layer="94">+</text>
<text x="13.208" y="62.23" size="1.778" layer="94">B+</text>
<text x="13.462" y="52.578" size="1.778" layer="94">B-</text>
<text x="6.096" y="44.196" size="1.778" layer="94">OUT-</text>
<text x="10.16" y="47.498" size="1.778" layer="94">OUT+</text>
<text x="12.954" y="70.104" size="1.778" layer="94">IN+</text>
<text x="64.77" y="70.358" size="1.778" layer="94">5V</text>
<text x="40.64" y="40.64" size="1.778" layer="94">VCC</text>
<text x="99.822" y="65.024" size="1.778" layer="94">VCC</text>
<text x="44.45" y="32.512" size="1.778" layer="94">SDA</text>
<text x="44.704" y="26.924" size="1.778" layer="94">SCL</text>
<text x="38.1" y="30.48" size="1.778" layer="94" rot="R90">GY-65</text>
<text x="5.08" y="53.34" size="1.778" layer="94" rot="R90">LiPo Charger</text>
<text x="78.74" y="38.1" size="1.778" layer="94" rot="R90">Nucleo-L432KC</text>
<text x="64.77" y="32.258" size="1.778" layer="94">D0</text>
<text x="64.516" y="27.178" size="1.778" layer="94">D1</text>
<text x="73.406" y="75.184" size="1.778" layer="94">USB</text>
<wire x1="4.826" y1="78.486" x2="4.826" y2="73.406" width="0.3048" layer="94"/>
<wire x1="4.826" y1="73.406" x2="14.986" y2="73.406" width="0.3048" layer="94"/>
<wire x1="14.986" y1="73.406" x2="14.986" y2="78.486" width="0.3048" layer="94"/>
<text x="7.112" y="74.93" size="1.778" layer="94">USB</text>
<wire x1="99.06" y1="78.74" x2="132.08" y2="78.74" width="0.3048" layer="94"/>
<wire x1="132.08" y1="78.74" x2="132.08" y2="20.32" width="0.3048" layer="94"/>
<wire x1="132.08" y1="20.32" x2="99.06" y2="20.32" width="0.3048" layer="94"/>
<wire x1="99.06" y1="20.32" x2="99.06" y2="78.74" width="0.3048" layer="94"/>
<text x="81.28" y="65.024" size="1.778" layer="94">AREF</text>
<text x="82.804" y="59.944" size="1.778" layer="94">GND</text>
<text x="99.822" y="59.944" size="1.778" layer="94">GND</text>
<text x="100.076" y="54.864" size="1.778" layer="94">DIN</text>
<text x="84.836" y="54.864" size="1.778" layer="94">A6</text>
<text x="100.076" y="49.784" size="1.778" layer="94">CLK</text>
<text x="84.836" y="50.038" size="1.778" layer="94">A4</text>
<text x="84.836" y="44.704" size="1.778" layer="94">A3</text>
<text x="100.076" y="44.704" size="1.778" layer="94">CS</text>
<text x="100.076" y="39.624" size="1.778" layer="94">DC</text>
<text x="84.836" y="39.624" size="1.778" layer="94">A2</text>
<text x="84.836" y="34.798" size="1.778" layer="94">A1</text>
<text x="99.822" y="34.544" size="1.778" layer="94">RST</text>
<text x="100.076" y="29.718" size="1.778" layer="94">BUSY</text>
<text x="85.09" y="29.464" size="1.778" layer="94">A0</text>
<text x="109.474" y="20.828" size="1.27" layer="94">4.2inch e-Paper Module</text>
<wire x1="107.442" y1="76.2" x2="107.442" y2="22.86" width="0.3048" layer="94"/>
<wire x1="107.442" y1="22.86" x2="129.54" y2="22.86" width="0.3048" layer="94"/>
<wire x1="129.54" y1="22.86" x2="129.54" y2="76.2" width="0.3048" layer="94"/>
<wire x1="129.54" y1="76.2" x2="107.442" y2="76.2" width="0.3048" layer="94"/>
</plain>
<instances>
<instance part="BAT1" gate="G$1" x="30.48" y="58.42" smashed="yes" rot="R270">
<attribute name="NAME" x="35.56" y="62.23" size="1.778" layer="95" rot="R270"/>
<attribute name="VALUE" x="24.13" y="62.23" size="1.778" layer="96" rot="R270"/>
</instance>
<instance part="GND1" gate="1" x="43.18" y="10.16" smashed="yes">
<attribute name="VALUE" x="40.64" y="7.62" size="1.778" layer="96"/>
</instance>
</instances>
<busses>
</busses>
<nets>
<net name="N$1" class="0">
<segment>
<pinref part="BAT1" gate="G$1" pin="+"/>
<wire x1="30.48" y1="63.5" x2="17.78" y2="63.5" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$2" class="0">
<segment>
<pinref part="BAT1" gate="G$1" pin="-"/>
<wire x1="30.48" y1="53.34" x2="17.78" y2="53.34" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$3" class="0">
<segment>
<wire x1="17.78" y1="71.12" x2="63.5" y2="71.12" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$4" class="0">
<segment>
<wire x1="17.78" y1="48.26" x2="43.18" y2="48.26" width="0.1524" layer="91"/>
<wire x1="43.18" y1="48.26" x2="43.18" y2="43.18" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$5" class="0">
<segment>
<wire x1="50.8" y1="38.1" x2="63.5" y2="38.1" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$6" class="0">
<segment>
<wire x1="50.8" y1="33.02" x2="63.5" y2="33.02" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$7" class="0">
<segment>
<wire x1="50.8" y1="27.94" x2="63.5" y2="27.94" width="0.1524" layer="91"/>
</segment>
</net>
<net name="GND" class="0">
<segment>
<wire x1="10.16" y1="43.18" x2="10.16" y2="15.24" width="0.1524" layer="91"/>
<wire x1="10.16" y1="15.24" x2="43.18" y2="15.24" width="0.1524" layer="91"/>
<wire x1="43.18" y1="15.24" x2="76.2" y2="15.24" width="0.1524" layer="91"/>
<wire x1="76.2" y1="15.24" x2="76.2" y2="20.32" width="0.1524" layer="91"/>
<wire x1="43.18" y1="22.86" x2="43.18" y2="15.24" width="0.1524" layer="91"/>
<junction x="43.18" y="15.24"/>
<pinref part="GND1" gate="1" pin="GND"/>
<wire x1="43.18" y1="15.24" x2="43.18" y2="12.7" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$8" class="0">
<segment>
<wire x1="88.9" y1="66.04" x2="99.06" y2="66.04" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$9" class="0">
<segment>
<wire x1="88.9" y1="60.96" x2="99.06" y2="60.96" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$10" class="0">
<segment>
<wire x1="88.9" y1="55.88" x2="99.06" y2="55.88" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$11" class="0">
<segment>
<wire x1="88.9" y1="50.8" x2="99.06" y2="50.8" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$12" class="0">
<segment>
<wire x1="88.9" y1="45.72" x2="99.06" y2="45.72" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$13" class="0">
<segment>
<wire x1="88.9" y1="40.64" x2="99.06" y2="40.64" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$14" class="0">
<segment>
<wire x1="88.9" y1="35.56" x2="99.06" y2="35.56" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$15" class="0">
<segment>
<wire x1="88.9" y1="30.48" x2="99.06" y2="30.48" width="0.1524" layer="91"/>
</segment>
</net>
</nets>
</sheet>
</sheets>
</schematic>
</drawing>
<compatibility>
<note version="8.2" severity="warning">
Since Version 8.2, EAGLE supports online libraries. The ids
of those online libraries will not be understood (or retained)
with this version.
</note>
<note version="8.3" severity="warning">
Since Version 8.3, EAGLE supports URNs for individual library
assets (packages, symbols, and devices). The URNs of those assets
will not be understood (or retained) with this version.
</note>
<note version="8.3" severity="warning">
Since Version 8.3, EAGLE supports the association of 3D packages
with devices in libraries, schematics, and board files. Those 3D
packages will not be understood (or retained) with this version.
</note>
</compatibility>
</eagle>
