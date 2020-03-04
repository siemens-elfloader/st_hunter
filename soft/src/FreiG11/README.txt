/*****************************************************************************\
 *                   FreiG11 - An alternate NatICQ server                    *
 *                                                                           *
 * Copyright (C) 2012 Barracuda                                              *
 *                                                                           *
 * Original IG11 server written by:                                          *
 * Rts7, BoBa                                                                *
 *                                                                           *
 * This program is free software: you can redistribute it and/or modify it   *
 * under the terms of the GNU General Public License as published by the     *
 * Free Software Foundation, either version 3 of the License, or (at your    *
 * option) any later version.                                                *
 *                                                                           *
 * This program is distributed in the hope that it will be useful, but       *
 * WITHOUT ANY WARRANTY; without even the implied warranty of                *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General *
 * Public License for more details.                                          *
 *                                                                           *
 * You should have received a copy of the GNU General Public License along   *
 * with this program.  If not, see <http://www.gnu.org/licenses/>.           *
 *                                                                           *
 * What's this?                                                              *
 *                                                                           *
 * This is free software implementation of NatICQ gateway server - IG11.     *
 * It's based on IcqKid2 library, and written from scratch with no code from *
 * original IG11 server.                                                     *
 *                                                                           *
 * Well, what is NatICQ?                                                     *
 *                                                                           *
 * NatICQ is a ICQ client, written for Siemens mobile phones. It uses C as   *
 * language, so in contrast with Jimm-based clients it's fast and stable.    *
 *                                                                           *
 * How does it work?                                                         *
 *                                                                           *
 * To run NatICQ on phone, you should have SGOLD- or NEWSGOLD-based          *
 * (Benq-) Siemens mobile phone with installed patch Elfloader. When, on     *
 * startup NatICQ client start communication with IG11 server, which itself  *
 * communicates with OSCAR servers:                                          *
 *                                                                           *
 *    NatICQ    <---->    IG11    <---->  OSCAR                              *
 * To be short, IG11 acts as a proxy between NatICQ client and ICQ server.   *
 *                                                                           *
 * What's the benefits?                                                      *
 *                                                                           *
 * This scheme is used to achieve major traffic and speed gain. You can save *
 * about 95% (!) of traffic in comparsion with usual ICQ clients.            *
 *                                                                           *
 * What?! Why so much?!                                                      *
 *                                                                           *
 * Yea, of course this is because of stripped down protocol. Origial OSCAR   *
 * proto, for example, has message len of 1 Kb + lenght of text. NatICQ has  *
 * only 8 bytes of payload! And so on.                                       *
 *                                                                           *
 * Is there any client to platform other than Siemens?                       *
 *                                                                           *
 * Yes, there is C and Java command-line clients and Android client.         *
 *                                                                           *
\*****************************************************************************/
