<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE TS>
<TS version="2.0" language="it_IT">
<context>
    <name>XMPPChatMarkerCommand</name>
    <message>
        <location filename="src/XmppChatMarkerCommand.cpp" line="38"/>
        <source>ERROR: xmppChatMarker &lt;resource&gt; &lt;marker&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="src/XmppChatMarkerCommand.cpp" line="90"/>
        <source>ERROR: Failed to send packet</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="src/XmppChatMarkerCommand.cpp" line="95"/>
        <source>ERROR: Invalid marker</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="src/XmppChatMarkerCommand.cpp" line="100"/>
        <source>ERROR: The last message isn&apos;t Markable</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="src/XmppChatMarkerCommand.cpp" line="105"/>
        <source>ERROR: Can&apos;t find the last message received to mark</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="src/XmppChatMarkerCommand.cpp" line="110"/>
        <source>ERROR: Unknown resource</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="src/XmppChatMarkerCommand.cpp" line="118"/>
        <source>&gt; xmppChatMarker &lt;resource&gt; &lt;marker&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="src/XmppChatMarkerCommand.cpp" line="119"/>
        <source>Send a chat marker back for the last message</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="src/XmppChatMarkerCommand.cpp" line="120"/>
        <source>received from that resource</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="src/XmppChatMarkerCommand.cpp" line="121"/>
        <source>Valid markers: received, displayed, acknowledged</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>XMPPChatStateCommand</name>
    <message>
        <location filename="src/XmppChatStateCommand.cpp" line="38"/>
        <source>ERROR: xmppChatState &lt;resource&gt; &lt;toJID&gt; &lt;state&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="src/XmppChatStateCommand.cpp" line="92"/>
        <source>ERROR: Failed to send packet</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="src/XmppChatStateCommand.cpp" line="97"/>
        <source>ERROR: Invalid state</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="src/XmppChatStateCommand.cpp" line="102"/>
        <source>ERROR: Unknown resource</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="src/XmppChatStateCommand.cpp" line="110"/>
        <source>&gt; xmppChatState &lt;resource&gt; &lt;toJID&gt; &lt;state&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="src/XmppChatStateCommand.cpp" line="111"/>
        <source>Update the chat state for a user.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="src/XmppChatStateCommand.cpp" line="112"/>
        <source>Valid states: active, inactive, gone, composing, paused</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>XMPPConnectCommand</name>
    <message>
        <location filename="src/XmppConnectCommand.cpp" line="44"/>
        <source>ERROR: xmppConnect &lt;user&gt; &lt;pass&gt; &lt;resource&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="src/XmppConnectCommand.cpp" line="65"/>
        <source>&gt; xmppConnect &lt;user&gt; &lt;pass&gt; &lt;resourceId&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="src/XmppConnectCommand.cpp" line="66"/>
        <source>Connect to an XMPP server.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="src/XmppConnectCommand.cpp" line="80"/>
        <source>ERROR: Disconnected</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="src/XmppConnectCommand.cpp" line="87"/>
        <source>ERROR: QXmppError</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>XMPPCorrectCommand</name>
    <message>
        <location filename="src/XmppCorrectCommand.cpp" line="38"/>
        <source>ERROR: xmppCorrect &lt;resource&gt; &lt;optional: message&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="src/XmppCorrectCommand.cpp" line="69"/>
        <source>ERROR: Failed to send packet</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="src/XmppCorrectCommand.cpp" line="79"/>
        <source>ERROR: The previous message has no body to correct</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="src/XmppCorrectCommand.cpp" line="85"/>
        <source>ERROR: No previous sent message to correct</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="src/XmppCorrectCommand.cpp" line="90"/>
        <source>ERROR: Unknown resource</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="src/XmppCorrectCommand.cpp" line="98"/>
        <source>&gt; xmppCorrect &lt;resource&gt; &lt;optional: message&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="src/XmppCorrectCommand.cpp" line="99"/>
        <source>Corrects the last message sent. If &lt;message&gt; is missing</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="src/XmppCorrectCommand.cpp" line="100"/>
        <source>then it&apos;s treated as a deletion of the previous message.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>XMPPDisconnectCommand</name>
    <message>
        <location filename="src/XmppDisconnectCommand.cpp" line="37"/>
        <source>ERROR: xmppDisconnect &lt;resource&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="src/XmppDisconnectCommand.cpp" line="53"/>
        <source>ERROR: Unknown resource</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="src/XmppDisconnectCommand.cpp" line="61"/>
        <source>&gt; xmppDisconnect &lt;resource&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="src/XmppDisconnectCommand.cpp" line="62"/>
        <source>Disconnect a resource&apos;s connection.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>XMPPHelpCommand</name>
    <message>
        <location filename="src/XmppHelpCommand.cpp" line="39"/>
        <source>&gt; xmppHelp</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="src/XmppHelpCommand.cpp" line="40"/>
        <source>The XMPP addin can be used to control an XMPP</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="src/XmppHelpCommand.cpp" line="41"/>
        <source>connection and used to send messages, statues updates</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="src/XmppHelpCommand.cpp" line="42"/>
        <source>and other XEPs to fully test messaging clients.</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>XMPPMessageCommand</name>
    <message>
        <location filename="src/XmppMessageCommand.cpp" line="38"/>
        <source>ERROR: xmppMessage &lt;resource&gt; &lt;to&gt; &lt;message&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="src/XmppMessageCommand.cpp" line="61"/>
        <source>ERROR: Failed to send packet</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="src/XmppMessageCommand.cpp" line="70"/>
        <source>ERROR: Unknown resource</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="src/XmppMessageCommand.cpp" line="78"/>
        <source>&gt; xmppMessage &lt;resource&gt; &lt;to&gt; &lt;message&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="src/XmppMessageCommand.cpp" line="79"/>
        <source>Send a message to a user</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>XMPPPresenceCommand</name>
    <message>
        <location filename="src/XmppPresenceCommand.cpp" line="38"/>
        <source>ERROR: xmppPresence &lt;resource&gt; &lt;priority&gt; &lt;status&gt; &lt;message&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="src/XmppPresenceCommand.cpp" line="52"/>
        <source>ERROR: Priority must be an integer</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="src/XmppPresenceCommand.cpp" line="90"/>
        <source>ERROR: Unknown presence status type</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="src/XmppPresenceCommand.cpp" line="99"/>
        <source>ERROR: Failed to send packet</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="src/XmppPresenceCommand.cpp" line="106"/>
        <source>ERROR: Unknown resource</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="src/XmppPresenceCommand.cpp" line="114"/>
        <source>&gt; xmppPresence &lt;resource&gt; &lt;priority&gt; &lt;status&gt; &lt;message&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="src/XmppPresenceCommand.cpp" line="115"/>
        <source>Set your XMPP status (online, away, xa, dnd, chat, invisible)</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>XMPPReachabilityCommand</name>
    <message>
        <location filename="src/XmppReachabilityCommand.cpp" line="39"/>
        <source>ERROR: xmppReachability &lt;resource&gt; &lt;address&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="src/XmppReachabilityCommand.cpp" line="70"/>
        <source>ERROR: Failed to send packet</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="src/XmppReachabilityCommand.cpp" line="75"/>
        <source>ERROR: Unknown resource</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="src/XmppReachabilityCommand.cpp" line="83"/>
        <source>&gt; xmppReachability &lt;resource&gt; &lt;address&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="src/XmppReachabilityCommand.cpp" line="84"/>
        <source>Send a reachability address to a user</source>
        <translation type="unfinished"></translation>
    </message>
</context>
<context>
    <name>XMPPTestCommand</name>
    <message>
        <location filename="src/XmppTestCommand.cpp" line="38"/>
        <source>ERROR: xmppTest &lt;resource&gt; &lt;field&gt; &lt;optional: value&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="src/XmppTestCommand.cpp" line="158"/>
        <source>ERROR: Value is {</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="src/XmppTestCommand.cpp" line="160"/>
        <source>} but expected {</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="src/XmppTestCommand.cpp" line="162"/>
        <source>}</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="src/XmppTestCommand.cpp" line="168"/>
        <source>ERROR: Unknown property</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="src/XmppTestCommand.cpp" line="173"/>
        <source>ERROR: Couldn&apos;t find the last received message</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="src/XmppTestCommand.cpp" line="178"/>
        <source>ERROR: Unknown resource</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="src/XmppTestCommand.cpp" line="186"/>
        <source>&gt; xmppTest &lt;resource&gt; &lt;field&gt; &lt;optional: value&gt;</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="src/XmppTestCommand.cpp" line="187"/>
        <source>Test a field in the last received message</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="src/XmppTestCommand.cpp" line="188"/>
        <source>If &lt;value&gt; is missing it&apos;ll test for null/empty</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="src/XmppTestCommand.cpp" line="189"/>
        <source>Supported &lt;value&gt; options:</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="src/XmppTestCommand.cpp" line="190"/>
        <source>body</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="src/XmppTestCommand.cpp" line="191"/>
        <source>attentionRequired</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="src/XmppTestCommand.cpp" line="192"/>
        <source>mucInvitationJid</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="src/XmppTestCommand.cpp" line="193"/>
        <source>mucInvitationPassword</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="src/XmppTestCommand.cpp" line="194"/>
        <source>mucInvitationReason</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="src/XmppTestCommand.cpp" line="195"/>
        <source>receiptId</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="src/XmppTestCommand.cpp" line="196"/>
        <source>stamp</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="src/XmppTestCommand.cpp" line="197"/>
        <source>state</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="src/XmppTestCommand.cpp" line="198"/>
        <source>subject</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="src/XmppTestCommand.cpp" line="199"/>
        <source>thread</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="src/XmppTestCommand.cpp" line="200"/>
        <source>type</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="src/XmppTestCommand.cpp" line="201"/>
        <source>hasForward</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="src/XmppTestCommand.cpp" line="202"/>
        <source>fordwardBody</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="src/XmppTestCommand.cpp" line="203"/>
        <source>hasCarbon</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="src/XmppTestCommand.cpp" line="204"/>
        <source>carbonBody</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="src/XmppTestCommand.cpp" line="205"/>
        <source>markable</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="src/XmppTestCommand.cpp" line="206"/>
        <source>markedId</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="src/XmppTestCommand.cpp" line="207"/>
        <source>markedThread</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="src/XmppTestCommand.cpp" line="208"/>
        <source>isReplace</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <location filename="src/XmppTestCommand.cpp" line="209"/>
        <source>replaceId</source>
        <translation type="unfinished"></translation>
    </message>
</context>
</TS>
