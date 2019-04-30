<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE eagle SYSTEM "eagle.dtd">
<eagle version="9.3.2">
<drawing>
<settings>
<setting alwaysvectorfont="no"/>
<setting verticaltext="up"/>
</settings>
<grid distance="0.0393700787" unitdist="inch" unit="inch" style="lines" multiple="1" display="yes" altdistance="0.1" altunitdist="mm" altunit="mm"/>
<layers>
<layer number="1" name="Top" color="4" fill="1" visible="no" active="no"/>
<layer number="2" name="Route2" color="16" fill="1" visible="no" active="no"/>
<layer number="3" name="Route3" color="17" fill="1" visible="no" active="no"/>
<layer number="4" name="Route4" color="18" fill="1" visible="no" active="no"/>
<layer number="5" name="Route5" color="19" fill="1" visible="no" active="no"/>
<layer number="6" name="Route6" color="25" fill="1" visible="no" active="no"/>
<layer number="7" name="Route7" color="26" fill="1" visible="no" active="no"/>
<layer number="8" name="Route8" color="27" fill="1" visible="no" active="no"/>
<layer number="9" name="Route9" color="28" fill="1" visible="no" active="no"/>
<layer number="10" name="Route10" color="29" fill="1" visible="no" active="no"/>
<layer number="11" name="Route11" color="30" fill="1" visible="no" active="no"/>
<layer number="12" name="Route12" color="20" fill="1" visible="no" active="no"/>
<layer number="13" name="Route13" color="21" fill="1" visible="no" active="no"/>
<layer number="14" name="Route14" color="22" fill="1" visible="no" active="no"/>
<layer number="15" name="Route15" color="23" fill="1" visible="no" active="no"/>
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
<layer number="95" name="Names" color="7" fill="1" visible="yes" active="yes"/>
<layer number="96" name="Values" color="7" fill="1" visible="yes" active="yes"/>
<layer number="97" name="Info" color="7" fill="1" visible="yes" active="yes"/>
<layer number="98" name="Guide" color="6" fill="1" visible="yes" active="yes"/>
</layers>
<schematic xreflabel="%F%N/%S.%C%R" xrefpart="/%S.%C%R">
<libraries>
<library name="Arduino-clone">
<description>Arduino Clone pinheaders
By cl@xganon.com
http://www.xganon.com</description>
<packages>
<package name="NANO">
<pad name="TX0" x="5.08" y="-8.89" drill="0.8" shape="long"/>
<pad name="RX1" x="5.08" y="-6.35" drill="0.8" shape="long"/>
<pad name="RST1" x="5.08" y="-3.81" drill="0.8" shape="long"/>
<pad name="GND1" x="5.08" y="-1.27" drill="0.8" shape="long"/>
<pad name="D2" x="5.08" y="1.27" drill="0.8" shape="long"/>
<pad name="D3" x="5.08" y="3.81" drill="0.8" shape="long"/>
<pad name="D4" x="5.08" y="6.35" drill="0.8" shape="long"/>
<pad name="D5" x="5.08" y="8.89" drill="0.8" shape="long"/>
<pad name="D6" x="5.08" y="11.43" drill="0.8" shape="long"/>
<pad name="D7" x="5.08" y="13.97" drill="0.8" shape="long"/>
<pad name="D8" x="5.08" y="16.51" drill="0.8" shape="long"/>
<pad name="D9" x="5.08" y="19.05" drill="0.8" shape="long"/>
<pad name="RAW" x="-10.16" y="-8.89" drill="0.8" shape="long"/>
<pad name="GND" x="-10.16" y="-6.35" drill="0.8" shape="long"/>
<pad name="RST" x="-10.16" y="-3.81" drill="0.8" shape="long"/>
<pad name="A3" x="-10.16" y="11.43" drill="0.8" shape="long"/>
<pad name="A2" x="-10.16" y="13.97" drill="0.8" shape="long"/>
<pad name="A1" x="-10.16" y="16.51" drill="0.8" shape="long"/>
<pad name="A0" x="-10.16" y="19.05" drill="0.8" shape="long"/>
<pad name="D13" x="-10.16" y="26.67" drill="0.8" shape="long"/>
<pad name="D12" x="5.08" y="26.67" drill="0.8" shape="long"/>
<pad name="D11" x="5.08" y="24.13" drill="0.8" shape="long"/>
<pad name="D10" x="5.08" y="21.59" drill="0.8" shape="long"/>
<pad name="3.3V" x="-10.16" y="24.13" drill="0.8" shape="long" rot="R180"/>
<pad name="AREF" x="-10.16" y="21.59" drill="0.8" shape="long" rot="R180"/>
<pad name="5V" x="-10.16" y="-1.27" drill="0.8" shape="long"/>
<pad name="ICSP2" x="-5.08" y="-7.62" drill="0.8" rot="R90"/>
<pad name="ICSP4" x="-2.54" y="-7.62" drill="0.8" rot="R90"/>
<pad name="ICSP6" x="0" y="-7.62" drill="0.8" rot="R90"/>
<pad name="A4" x="-10.16" y="8.89" drill="0.8" shape="long" rot="R180"/>
<pad name="A5" x="-10.16" y="6.35" drill="0.8" shape="long" rot="R180"/>
<pad name="A6" x="-10.16" y="3.81" drill="0.8" shape="long" rot="R180"/>
<pad name="A7" x="-10.16" y="1.27" drill="0.8" shape="long" rot="R180"/>
<pad name="ICSP1" x="-5.08" y="-10.16" drill="0.8" rot="R90"/>
<pad name="ICSP3" x="-2.54" y="-10.16" drill="0.8" rot="R90"/>
<pad name="ICSP5" x="0" y="-10.16" drill="0.8" rot="R90"/>
</package>
</packages>
<symbols>
<symbol name="NANO">
<pin name="3.3V" x="-22.86" y="12.7" visible="pin" length="middle"/>
<pin name="AREF" x="-22.86" y="10.16" visible="pin" length="middle"/>
<pin name="ICSP1" x="-10.16" y="-27.94" visible="off" length="point" rot="R270"/>
<pin name="ICSP2" x="-10.16" y="-25.4" visible="off" length="point" rot="R270"/>
<pin name="ICSP3" x="-7.62" y="-27.94" visible="off" length="point" rot="R270"/>
<pin name="ICSP4" x="-7.62" y="-25.4" visible="off" length="point" rot="R270"/>
<pin name="TX0" x="10.16" y="-20.32" visible="pin" length="middle" rot="R180"/>
<pin name="RX1" x="10.16" y="-17.78" visible="pin" length="middle" rot="R180"/>
<pin name="RST1" x="10.16" y="-15.24" visible="pin" length="middle" rot="R180"/>
<pin name="GND2" x="10.16" y="-12.7" visible="pin" length="middle" rot="R180"/>
<pin name="D2" x="10.16" y="-10.16" visible="pin" length="middle" rot="R180"/>
<pin name="D3" x="10.16" y="-7.62" visible="pin" length="middle" rot="R180"/>
<pin name="D4" x="10.16" y="-5.08" visible="pin" length="middle" rot="R180"/>
<pin name="D5" x="10.16" y="-2.54" visible="pin" length="middle" rot="R180"/>
<pin name="D6" x="10.16" y="0" visible="pin" length="middle" rot="R180"/>
<pin name="D7" x="10.16" y="2.54" visible="pin" length="middle" rot="R180"/>
<pin name="D8" x="10.16" y="5.08" visible="pin" length="middle" rot="R180"/>
<pin name="D9" x="10.16" y="7.62" visible="pin" length="middle" rot="R180"/>
<pin name="RAW" x="-22.86" y="-20.32" visible="pin" length="middle"/>
<pin name="GND" x="-22.86" y="-17.78" visible="pin" length="middle"/>
<pin name="RST" x="-22.86" y="-15.24" visible="pin" length="middle"/>
<pin name="5.5V" x="-22.86" y="-12.7" visible="pin" length="middle"/>
<pin name="A3" x="-22.86" y="0" visible="pin" length="middle"/>
<pin name="A2" x="-22.86" y="2.54" visible="pin" length="middle"/>
<pin name="A1" x="-22.86" y="5.08" visible="pin" length="middle"/>
<pin name="A0" x="-22.86" y="7.62" visible="pin" length="middle"/>
<pin name="D13" x="-22.86" y="15.24" visible="pin" length="middle"/>
<pin name="D12" x="10.16" y="15.24" visible="pin" length="middle" rot="R180"/>
<pin name="D11" x="10.16" y="12.7" visible="pin" length="middle" rot="R180"/>
<pin name="D10" x="10.16" y="10.16" visible="pin" length="middle" rot="R180"/>
<pin name="A7" x="-22.86" y="-10.16" visible="pin" length="middle"/>
<pin name="A6" x="-22.86" y="-7.62" visible="pin" length="middle"/>
<pin name="A5" x="-22.86" y="-5.08" visible="pin" length="middle"/>
<pin name="A4" x="-22.86" y="-2.54" visible="pin" length="middle"/>
<pin name="ICSP5" x="-5.08" y="-27.94" visible="off" length="point" rot="R270"/>
<pin name="ICSP6" x="-5.08" y="-25.4" visible="off" length="point" rot="R270"/>
<wire x1="-17.78" y1="17.78" x2="-17.78" y2="-30.48" width="0.254" layer="94"/>
<wire x1="-17.78" y1="-30.48" x2="5.08" y2="-30.48" width="0.254" layer="94"/>
<wire x1="5.08" y1="-30.48" x2="5.08" y2="17.78" width="0.254" layer="94"/>
<wire x1="5.08" y1="17.78" x2="-17.78" y2="17.78" width="0.254" layer="94"/>
<text x="-12.7" y="20.32" size="1.778" layer="95">Arduino Nano</text>
</symbol>
</symbols>
<devicesets>
<deviceset name="NANO">
<description>Arduino Nano</description>
<gates>
<gate name="G$1" symbol="NANO" x="7.62" y="2.54"/>
</gates>
<devices>
<device name="" package="NANO">
<connects>
<connect gate="G$1" pin="3.3V" pad="3.3V"/>
<connect gate="G$1" pin="5.5V" pad="5V"/>
<connect gate="G$1" pin="A0" pad="A0"/>
<connect gate="G$1" pin="A1" pad="A1"/>
<connect gate="G$1" pin="A2" pad="A2"/>
<connect gate="G$1" pin="A3" pad="A3"/>
<connect gate="G$1" pin="A4" pad="A4"/>
<connect gate="G$1" pin="A5" pad="A5"/>
<connect gate="G$1" pin="A6" pad="A6"/>
<connect gate="G$1" pin="A7" pad="A7"/>
<connect gate="G$1" pin="AREF" pad="AREF"/>
<connect gate="G$1" pin="D10" pad="D10"/>
<connect gate="G$1" pin="D11" pad="D11"/>
<connect gate="G$1" pin="D12" pad="D12"/>
<connect gate="G$1" pin="D13" pad="D13"/>
<connect gate="G$1" pin="D2" pad="D2"/>
<connect gate="G$1" pin="D3" pad="D3"/>
<connect gate="G$1" pin="D4" pad="D4"/>
<connect gate="G$1" pin="D5" pad="D5"/>
<connect gate="G$1" pin="D6" pad="D6"/>
<connect gate="G$1" pin="D7" pad="D7"/>
<connect gate="G$1" pin="D8" pad="D8"/>
<connect gate="G$1" pin="D9" pad="D9"/>
<connect gate="G$1" pin="GND" pad="GND"/>
<connect gate="G$1" pin="GND2" pad="GND1"/>
<connect gate="G$1" pin="ICSP1" pad="ICSP1"/>
<connect gate="G$1" pin="ICSP2" pad="ICSP2"/>
<connect gate="G$1" pin="ICSP3" pad="ICSP3"/>
<connect gate="G$1" pin="ICSP4" pad="ICSP4"/>
<connect gate="G$1" pin="ICSP5" pad="ICSP5"/>
<connect gate="G$1" pin="ICSP6" pad="ICSP6"/>
<connect gate="G$1" pin="RAW" pad="RAW"/>
<connect gate="G$1" pin="RST" pad="RST"/>
<connect gate="G$1" pin="RST1" pad="RST1"/>
<connect gate="G$1" pin="RX1" pad="RX1"/>
<connect gate="G$1" pin="TX0" pad="TX0"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
</devices>
</deviceset>
</devicesets>
</library>
<library name="Apa102">
<packages>
<package name="APA102">
<smd name="1_DI" x="-2.27" y="1.905" dx="1.5" dy="1" layer="1" rot="R180"/>
<smd name="2_CI" x="-2.27" y="-0.095" dx="1.5" dy="1" layer="1" rot="R180"/>
<smd name="3_GND" x="-2.27" y="-2.095" dx="1.5" dy="1" layer="1" rot="R180"/>
<smd name="6_DO" x="2.316" y="1.905" dx="1.5" dy="1" layer="1" rot="R180"/>
<smd name="5_CO" x="2.316" y="-0.095" dx="1.5" dy="1" layer="1" rot="R180"/>
<smd name="4_VCC" x="2.316" y="-2.095" dx="1.5" dy="1" layer="1" rot="R180"/>
<pad name="P$1" x="-5.08" y="2.54" drill="0.6"/>
<pad name="P$2" x="-5.08" y="0" drill="0.6"/>
<pad name="P$3" x="-5.08" y="-2.54" drill="0.6"/>
<pad name="P$4" x="5.08" y="2.54" drill="0.6"/>
<pad name="P$5" x="5.08" y="0" drill="0.6"/>
<pad name="P$6" x="5.08" y="-2.54" drill="0.6"/>
<wire x1="-6" y1="3.5" x2="6" y2="3.5" width="0.127" layer="21"/>
<wire x1="6" y1="3.5" x2="6" y2="-3.5" width="0.127" layer="21"/>
<wire x1="6" y1="-3.5" x2="-6" y2="-3.5" width="0.127" layer="21"/>
<wire x1="-6" y1="-3.5" x2="-6" y2="3.5" width="0.127" layer="21"/>
<circle x="-3.5" y="3" radius="0.5" width="0.127" layer="21"/>
<text x="-3.81" y="5.08" size="1.27" layer="25">&gt;NAME</text>
</package>
<package name="JSTCONN">
<pad name="1_GND" x="0" y="2.54" drill="0.6"/>
<pad name="2_DATA" x="0" y="0" drill="0.6"/>
<pad name="3_VCC" x="0" y="-2.54" drill="0.6"/>
<wire x1="-2.54" y1="5.08" x2="2.54" y2="5.08" width="0.127" layer="21"/>
<wire x1="2.54" y1="5.08" x2="2.54" y2="-5.08" width="0.127" layer="21"/>
<wire x1="2.54" y1="-5.08" x2="-2.54" y2="-5.08" width="0.127" layer="21"/>
<wire x1="-2.54" y1="-5.08" x2="-2.54" y2="5.08" width="0.127" layer="21"/>
<circle x="-1.778" y="4.318" radius="0.567959375" width="0.127" layer="21"/>
<text x="-3.302" y="6.096" size="1.27" layer="25">&gt;NAME</text>
</package>
<package name="GCC">
<pad name="GND@1" x="-5" y="0" drill="0.6"/>
<pad name="GND@2" x="-3" y="0" drill="0.6"/>
<pad name="GND@3" x="-1" y="0" drill="0.6"/>
<pad name="4_DATA" x="1" y="0" drill="0.6"/>
<pad name="5_5V" x="3" y="0" drill="0.6"/>
<pad name="6_3.3V" x="5" y="0" drill="0.6"/>
<wire x1="-6" y1="1" x2="6" y2="1" width="0.127" layer="21"/>
<wire x1="6" y1="1" x2="6" y2="-1" width="0.127" layer="21"/>
<wire x1="6" y1="-1" x2="-6" y2="-1" width="0.127" layer="21"/>
<wire x1="-6" y1="-1" x2="-6" y2="1" width="0.127" layer="21"/>
<circle x="-6" y="1" radius="0.5" width="0.127" layer="21"/>
<text x="-3" y="2.5" size="1.27" layer="25">&gt;NAME</text>
</package>
</packages>
<symbols>
<symbol name="APA102">
<wire x1="-10.16" y1="5.08" x2="10.16" y2="5.08" width="0.254" layer="94"/>
<wire x1="10.16" y1="5.08" x2="10.16" y2="-5.08" width="0.254" layer="94"/>
<wire x1="10.16" y1="-5.08" x2="-10.16" y2="-5.08" width="0.254" layer="94"/>
<wire x1="-10.16" y1="-5.08" x2="-10.16" y2="5.08" width="0.254" layer="94"/>
<pin name="1_DI" x="-15.24" y="2.54" length="middle"/>
<pin name="2_CI" x="-15.24" y="0" length="middle"/>
<pin name="3_GND" x="-15.24" y="-2.54" length="middle"/>
<pin name="4_VCC" x="15.24" y="-2.54" length="middle" rot="R180"/>
<pin name="5_CO" x="15.24" y="0" length="middle" rot="R180"/>
<pin name="6_DO" x="15.24" y="2.54" length="middle" rot="R180"/>
<text x="-5.08" y="7.62" size="1.27" layer="95">&gt;NAME</text>
</symbol>
<symbol name="JSTCONN">
<wire x1="-2.54" y1="5.08" x2="10.16" y2="5.08" width="0.254" layer="94"/>
<wire x1="10.16" y1="5.08" x2="10.16" y2="-5.08" width="0.254" layer="94"/>
<wire x1="10.16" y1="-5.08" x2="-2.54" y2="-5.08" width="0.254" layer="94"/>
<wire x1="-2.54" y1="-5.08" x2="-2.54" y2="5.08" width="0.254" layer="94"/>
<pin name="1_GND" x="-7.62" y="2.54" length="middle"/>
<pin name="2_DATA" x="-7.62" y="0" length="middle"/>
<pin name="3_VCC" x="-7.62" y="-2.54" length="middle"/>
<text x="-2.54" y="7.62" size="1.27" layer="95">&gt;NAME</text>
</symbol>
<symbol name="GCC">
<wire x1="-7" y1="2" x2="7" y2="2" width="0.254" layer="94"/>
<wire x1="7" y1="2" x2="7" y2="-10" width="0.254" layer="94"/>
<wire x1="7" y1="-10" x2="-7" y2="-10" width="0.254" layer="94"/>
<wire x1="-7" y1="-10" x2="-7" y2="2" width="0.254" layer="94"/>
<pin name="1_GND" x="-5" y="4" length="short" rot="R270"/>
<pin name="2_GND" x="-3" y="4" length="short" rot="R270"/>
<pin name="3_GND" x="-1" y="4" length="short" rot="R270"/>
<pin name="4_DATA" x="1" y="4" length="short" rot="R270"/>
<pin name="5_5V" x="3" y="4" length="short" rot="R270"/>
<pin name="6_3.3V" x="5" y="4" length="short" rot="R270"/>
<text x="-10.54" y="-6.54" size="1.27" layer="95" rot="R90">&gt;NAME</text>
</symbol>
<symbol name="JSTCONNFEMALE">
<wire x1="-2.54" y1="5.08" x2="-2.54" y2="-5.08" width="0.254" layer="94"/>
<wire x1="-2.54" y1="-5.08" x2="12.7" y2="-5.08" width="0.254" layer="94"/>
<wire x1="12.7" y1="-5.08" x2="12.7" y2="5.08" width="0.254" layer="94"/>
<wire x1="12.7" y1="5.08" x2="-2.54" y2="5.08" width="0.254" layer="94"/>
<pin name="1_GND" x="17.78" y="2.54" length="middle" rot="R180"/>
<pin name="2_DATA" x="17.78" y="0" length="middle" rot="R180"/>
<pin name="3_VCC" x="17.78" y="-2.54" length="middle" rot="R180"/>
<text x="0" y="7.62" size="1.778" layer="95">&gt;NAME</text>
</symbol>
</symbols>
<devicesets>
<deviceset name="APA102">
<gates>
<gate name="G$1" symbol="APA102" x="0" y="0"/>
</gates>
<devices>
<device name="" package="APA102">
<connects>
<connect gate="G$1" pin="1_DI" pad="1_DI P$1"/>
<connect gate="G$1" pin="2_CI" pad="2_CI P$2"/>
<connect gate="G$1" pin="3_GND" pad="3_GND P$3"/>
<connect gate="G$1" pin="4_VCC" pad="4_VCC P$4"/>
<connect gate="G$1" pin="5_CO" pad="5_CO P$5"/>
<connect gate="G$1" pin="6_DO" pad="6_DO P$6"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
</devices>
</deviceset>
<deviceset name="JSTCONNMALE">
<gates>
<gate name="G$1" symbol="JSTCONN" x="-2.54" y="0"/>
</gates>
<devices>
<device name="" package="JSTCONN">
<connects>
<connect gate="G$1" pin="1_GND" pad="1_GND"/>
<connect gate="G$1" pin="2_DATA" pad="2_DATA"/>
<connect gate="G$1" pin="3_VCC" pad="3_VCC"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
</devices>
</deviceset>
<deviceset name="GCCHEADER">
<gates>
<gate name="G$1" symbol="GCC" x="0" y="5.08"/>
</gates>
<devices>
<device name="" package="GCC">
<connects>
<connect gate="G$1" pin="1_GND" pad="GND@1"/>
<connect gate="G$1" pin="2_GND" pad="GND@2"/>
<connect gate="G$1" pin="3_GND" pad="GND@3"/>
<connect gate="G$1" pin="4_DATA" pad="4_DATA"/>
<connect gate="G$1" pin="5_5V" pad="5_5V"/>
<connect gate="G$1" pin="6_3.3V" pad="6_3.3V"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
</devices>
</deviceset>
<deviceset name="JSTCONNFEMALE">
<gates>
<gate name="G$1" symbol="JSTCONNFEMALE" x="0" y="0"/>
</gates>
<devices>
<device name="" package="JSTCONN">
<connects>
<connect gate="G$1" pin="1_GND" pad="1_GND"/>
<connect gate="G$1" pin="2_DATA" pad="2_DATA"/>
<connect gate="G$1" pin="3_VCC" pad="3_VCC"/>
</connects>
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
<part name="U$1" library="Arduino-clone" deviceset="NANO" device=""/>
<part name="DOTSTAR1" library="Apa102" deviceset="APA102" device=""/>
<part name="DOTSTAR2" library="Apa102" deviceset="APA102" device=""/>
<part name="DOTSTAR3" library="Apa102" deviceset="APA102" device=""/>
<part name="DOTSTAR4" library="Apa102" deviceset="APA102" device=""/>
<part name="DOTSTAR5" library="Apa102" deviceset="APA102" device=""/>
<part name="JSTCONNMALE" library="Apa102" deviceset="JSTCONNMALE" device=""/>
<part name="U$2" library="Apa102" deviceset="GCCHEADER" device=""/>
<part name="JSTCONNFEMALE" library="Apa102" deviceset="JSTCONNFEMALE" device=""/>
</parts>
<sheets>
<sheet>
<plain>
</plain>
<instances>
<instance part="U$1" gate="G$1" x="7.62" y="53.34" smashed="yes"/>
<instance part="DOTSTAR1" gate="G$1" x="73.66" y="58.42" smashed="yes">
<attribute name="NAME" x="68.58" y="66.04" size="1.27" layer="95"/>
</instance>
<instance part="DOTSTAR2" gate="G$1" x="116.84" y="58.42" smashed="yes">
<attribute name="NAME" x="111.76" y="66.04" size="1.27" layer="95"/>
</instance>
<instance part="DOTSTAR3" gate="G$1" x="160.02" y="58.42" smashed="yes">
<attribute name="NAME" x="154.94" y="66.04" size="1.27" layer="95"/>
</instance>
<instance part="DOTSTAR4" gate="G$1" x="200.66" y="58.42" smashed="yes">
<attribute name="NAME" x="195.58" y="66.04" size="1.27" layer="95"/>
</instance>
<instance part="DOTSTAR5" gate="G$1" x="238.76" y="58.42" smashed="yes">
<attribute name="NAME" x="233.68" y="66.04" size="1.27" layer="95"/>
</instance>
<instance part="JSTCONNMALE" gate="G$1" x="36.56" y="111.76" smashed="yes" rot="R270">
<attribute name="NAME" x="44.18" y="114.3" size="1.27" layer="95" rot="R270"/>
</instance>
<instance part="U$2" gate="G$1" x="38.354" y="141.446" smashed="yes" rot="R180">
<attribute name="NAME" x="48.894" y="147.986" size="1.27" layer="95" rot="R270"/>
</instance>
<instance part="JSTCONNFEMALE" gate="G$1" x="36.6" y="98.5" smashed="yes" rot="R270">
<attribute name="NAME" x="44.22" y="98.5" size="1.778" layer="95" rot="R270"/>
</instance>
</instances>
<busses>
</busses>
<nets>
<net name="N$7" class="0">
<segment>
<pinref part="DOTSTAR1" gate="G$1" pin="3_GND"/>
<wire x1="58.42" y1="55.88" x2="58.42" y2="50" width="0.1524" layer="91"/>
<wire x1="58.42" y1="50" x2="58" y2="50" width="0.1524" layer="91"/>
<pinref part="DOTSTAR2" gate="G$1" pin="3_GND"/>
<wire x1="58.42" y1="50" x2="101.6" y2="50" width="0.1524" layer="91"/>
<wire x1="101.6" y1="55.88" x2="101.6" y2="50" width="0.1524" layer="91"/>
<pinref part="DOTSTAR3" gate="G$1" pin="3_GND"/>
<wire x1="101.6" y1="50" x2="144.78" y2="50" width="0.1524" layer="91"/>
<wire x1="144.78" y1="55.88" x2="144.78" y2="50" width="0.1524" layer="91"/>
<pinref part="DOTSTAR4" gate="G$1" pin="3_GND"/>
<wire x1="144.78" y1="50" x2="185.42" y2="50" width="0.1524" layer="91"/>
<wire x1="185.42" y1="55.88" x2="185.42" y2="50" width="0.1524" layer="91"/>
<wire x1="185.42" y1="50" x2="223.52" y2="50" width="0.1524" layer="91"/>
<pinref part="DOTSTAR5" gate="G$1" pin="3_GND"/>
<wire x1="223.52" y1="50" x2="223.52" y2="55.88" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$8" class="0">
<segment>
<pinref part="DOTSTAR1" gate="G$1" pin="2_CI"/>
<wire x1="58.42" y1="58.42" x2="45" y2="58.42" width="0.1524" layer="91"/>
<wire x1="45" y1="58.42" x2="45" y2="75" width="0.1524" layer="91"/>
<pinref part="U$1" gate="G$1" pin="D13"/>
<wire x1="45" y1="75" x2="-15.24" y2="75" width="0.1524" layer="91"/>
<wire x1="-15.24" y1="75" x2="-15.24" y2="68.58" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$9" class="0">
<segment>
<pinref part="DOTSTAR1" gate="G$1" pin="1_DI"/>
<pinref part="U$1" gate="G$1" pin="D11"/>
<wire x1="58.42" y1="60.96" x2="58.42" y2="66.04" width="0.1524" layer="91"/>
<wire x1="58.42" y1="66.04" x2="17.78" y2="66.04" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$10" class="0">
<segment>
<pinref part="DOTSTAR1" gate="G$1" pin="6_DO"/>
<pinref part="DOTSTAR2" gate="G$1" pin="1_DI"/>
<wire x1="88.9" y1="60.96" x2="101.6" y2="60.96" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$11" class="0">
<segment>
<pinref part="DOTSTAR1" gate="G$1" pin="5_CO"/>
<pinref part="DOTSTAR2" gate="G$1" pin="2_CI"/>
<wire x1="88.9" y1="58.42" x2="101.6" y2="58.42" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$12" class="0">
<segment>
<pinref part="DOTSTAR1" gate="G$1" pin="4_VCC"/>
<wire x1="88.9" y1="55.88" x2="89" y2="55.88" width="0.1524" layer="91"/>
<wire x1="89" y1="55.88" x2="89" y2="46" width="0.1524" layer="91"/>
<pinref part="DOTSTAR2" gate="G$1" pin="4_VCC"/>
<wire x1="89" y1="46" x2="132.08" y2="46" width="0.1524" layer="91"/>
<wire x1="132.08" y1="55.88" x2="132.08" y2="46" width="0.1524" layer="91"/>
<wire x1="132.08" y1="46" x2="175.26" y2="46" width="0.1524" layer="91"/>
<pinref part="DOTSTAR3" gate="G$1" pin="4_VCC"/>
<wire x1="175.26" y1="55.88" x2="175.26" y2="46" width="0.1524" layer="91"/>
<wire x1="175.26" y1="46" x2="216" y2="46" width="0.1524" layer="91"/>
<pinref part="DOTSTAR4" gate="G$1" pin="4_VCC"/>
<wire x1="215.9" y1="55.88" x2="216" y2="55.88" width="0.1524" layer="91"/>
<wire x1="216" y1="55.88" x2="216" y2="46" width="0.1524" layer="91"/>
<pinref part="DOTSTAR5" gate="G$1" pin="4_VCC"/>
<wire x1="254" y1="55.88" x2="254" y2="46" width="0.1524" layer="91"/>
<wire x1="254" y1="46" x2="215.9" y2="46" width="0.1524" layer="91"/>
<wire x1="215.9" y1="46" x2="215.9" y2="55.88" width="0.1524" layer="91"/>
<junction x="215.9" y="55.88"/>
</segment>
</net>
<net name="N$13" class="0">
<segment>
<pinref part="DOTSTAR2" gate="G$1" pin="6_DO"/>
<pinref part="DOTSTAR3" gate="G$1" pin="1_DI"/>
<wire x1="132.08" y1="60.96" x2="144.78" y2="60.96" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$14" class="0">
<segment>
<pinref part="DOTSTAR2" gate="G$1" pin="5_CO"/>
<pinref part="DOTSTAR3" gate="G$1" pin="2_CI"/>
<wire x1="132.08" y1="58.42" x2="144.78" y2="58.42" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$15" class="0">
<segment>
<pinref part="DOTSTAR3" gate="G$1" pin="6_DO"/>
<pinref part="DOTSTAR4" gate="G$1" pin="1_DI"/>
<wire x1="175.26" y1="60.96" x2="185.42" y2="60.96" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$16" class="0">
<segment>
<pinref part="DOTSTAR3" gate="G$1" pin="5_CO"/>
<pinref part="DOTSTAR4" gate="G$1" pin="2_CI"/>
<wire x1="175.26" y1="58.42" x2="185.42" y2="58.42" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$17" class="0">
<segment>
<pinref part="DOTSTAR4" gate="G$1" pin="6_DO"/>
<pinref part="DOTSTAR5" gate="G$1" pin="1_DI"/>
<wire x1="215.9" y1="60.96" x2="223.52" y2="60.96" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$18" class="0">
<segment>
<pinref part="DOTSTAR4" gate="G$1" pin="5_CO"/>
<pinref part="DOTSTAR5" gate="G$1" pin="2_CI"/>
<wire x1="215.9" y1="58.42" x2="223.52" y2="58.42" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$5" class="0">
<segment>
<pinref part="JSTCONNMALE" gate="G$1" pin="3_VCC"/>
<pinref part="U$2" gate="G$1" pin="5_5V"/>
<wire x1="35.354" y1="137.446" x2="35.354" y2="119.38" width="0.1524" layer="91"/>
<wire x1="35.354" y1="119.38" x2="34.02" y2="119.38" width="0.1524" layer="91"/>
<pinref part="JSTCONNFEMALE" gate="G$1" pin="3_VCC"/>
<wire x1="34.06" y1="80.72" x2="-19" y2="80.72" width="0.1524" layer="91"/>
<wire x1="-19" y1="80.72" x2="-19" y2="40.64" width="0.1524" layer="91"/>
<pinref part="U$1" gate="G$1" pin="5.5V"/>
<wire x1="-19" y1="40.64" x2="-15.24" y2="40.64" width="0.1524" layer="91"/>
<wire x1="34.02" y1="119.38" x2="34.02" y2="80.72" width="0.1524" layer="91"/>
<wire x1="34.02" y1="80.72" x2="34.06" y2="80.72" width="0.1524" layer="91"/>
<junction x="34.02" y="119.38"/>
<junction x="34.06" y="80.72"/>
</segment>
</net>
<net name="N$6" class="0">
<segment>
<pinref part="U$2" gate="G$1" pin="3_GND"/>
<wire x1="39.1" y1="137.446" x2="39.354" y2="137.446" width="0.1524" layer="91"/>
<pinref part="JSTCONNMALE" gate="G$1" pin="1_GND"/>
<wire x1="39.1" y1="119.38" x2="39.1" y2="137.446" width="0.1524" layer="91"/>
<pinref part="JSTCONNFEMALE" gate="G$1" pin="1_GND"/>
<wire x1="39.14" y1="80.72" x2="39.14" y2="42" width="0.1524" layer="91"/>
<pinref part="U$1" gate="G$1" pin="GND2"/>
<wire x1="39.14" y1="42" x2="17.78" y2="42" width="0.1524" layer="91"/>
<wire x1="17.78" y1="42" x2="17.78" y2="40.64" width="0.1524" layer="91"/>
<wire x1="39.1" y1="119.38" x2="39.1" y2="80.72" width="0.1524" layer="91"/>
<wire x1="39.1" y1="80.72" x2="39.14" y2="80.72" width="0.1524" layer="91"/>
<junction x="39.1" y="119.38"/>
<junction x="39.14" y="80.72"/>
</segment>
</net>
<net name="N$2" class="0">
<segment>
<wire x1="39" y1="55.88" x2="39" y2="56" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$3" class="0">
<segment>
<pinref part="JSTCONNFEMALE" gate="G$1" pin="2_DATA"/>
<wire x1="36.6" y1="80.72" x2="36.6" y2="55" width="0.1524" layer="91"/>
<pinref part="U$1" gate="G$1" pin="D7"/>
<wire x1="36.6" y1="55" x2="17.78" y2="55" width="0.1524" layer="91"/>
<wire x1="17.78" y1="55" x2="17.78" y2="55.88" width="0.1524" layer="91"/>
<pinref part="U$2" gate="G$1" pin="4_DATA"/>
<pinref part="JSTCONNMALE" gate="G$1" pin="2_DATA"/>
<wire x1="36.56" y1="119.38" x2="37.354" y2="119.38" width="0.1524" layer="91"/>
<wire x1="37.354" y1="119.38" x2="37.354" y2="137.446" width="0.1524" layer="91"/>
<wire x1="36.6" y1="80.72" x2="36.6" y2="119.38" width="0.1524" layer="91"/>
<wire x1="36.6" y1="119.38" x2="36.56" y2="119.38" width="0.1524" layer="91"/>
<junction x="36.6" y="80.72"/>
<junction x="36.56" y="119.38"/>
</segment>
</net>
</nets>
</sheet>
</sheets>
</schematic>
</drawing>
</eagle>
