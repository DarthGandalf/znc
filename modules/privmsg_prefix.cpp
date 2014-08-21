/*
 * Copyright (C) 2004-2014 ZNC, see the NOTICE file for details.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <znc/IRCNetwork.h>
#include <znc/Modules.h>

class CPrivMsgPrefixMod : public CModule {
public:
	MODCONSTRUCTOR(CPrivMsgPrefixMod) {}

	virtual EModRet OnUserMsg(CString& sTarget, CString& sMessage) {
		if (m_pNetwork && m_pNetwork->GetIRCSock() && !m_pNetwork->IsChan(sTarget)) {
			m_pNetwork->PutUser(":" + sTarget + "!prefix@privmsg.znc.in PRIVMSG " + m_pNetwork->GetIRCNick().GetNick() + " :-> " + sMessage, NULL, m_pClient);
		}

		return CONTINUE;
	}

	virtual EModRet OnUserAction(CString& sTarget, CString& sMessage) {
		if (m_pNetwork && m_pNetwork->GetIRCSock() && !m_pNetwork->IsChan(sTarget)) {
			m_pNetwork->PutUser(":" + sTarget + "!prefix@privmsg.znc.in PRIVMSG " + m_pNetwork->GetIRCNick().GetNick() + " :\x01" + "ACTION -> " + sMessage + "\x01", NULL, m_pClient);
		}

		return CONTINUE;
	}

	/*
	@time=2014-08-21T15:15:05.861Z :other!someident@notmyhost PRIVMSG me :This is a message sent to me.
	@time=2014-08-21T15:15:10.090Z :me!myident@myhost PRIVMSG other :This is a message sent by me.
	*/
	virtual EModRet OnPrivBufferPlayLine(CClient &Client, CString &sLine) {
		VCString vsSplit;
		sLine.Split(" ", vsSplit);

		bool changed = false;
		for (unsigned int a = 0; a < vsSplit.size(); a++) {
			if (vsSplit[a].WildCmp(":*!*@*")) {
				if (a + 3 >= vsSplit.size()) break;
				CString sFrom = vsSplit[a].TrimPrefix_n(":").Token(0, false, "!", false);
				CString sMask = vsSplit[a].Token(1, false, "!", false);
				// to = vsSplit[a+2]
				if (sFrom.Equals(Client.GetNick()) || sMask.Equals(Client.GetNickMask().Token(1, false, "!", false))) {
					vsSplit[a] = ":" + CString(vsSplit[a+2]) + "!prefix@privmsg.znc.in";
					vsSplit[a+2] = sFrom;
					CString sMessage = vsSplit[a+3].TrimPrefix_n(":");
					if (sMessage.StartsWith(CString("\x01") + "ACTION")) {
						vsSplit[a+3] = CString(":\x01") + "ACTION ->" + sMessage;
					} else {
						vsSplit[a+3] = ":-> " + sMessage;
					}
					changed = true;
				}
			}
		}
		if (changed)  {
			sLine = CString(" ").Join(vsSplit.begin(), vsSplit.end());
		}
		return CONTINUE;
	}
};

USERMODULEDEFS(CPrivMsgPrefixMod, "Send outgoing PRIVMSGs and CTCP ACTIONs to other clients using ugly prefixes")

