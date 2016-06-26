## Mockups

![dashboard / thread list](http://i.imgur.com/MI7If9T.png)

![inside a thread](http://i.imgur.com/JB8PLp5.png)

## Data Model

Object | Purpose
-------|--------
Mun | User account.
Muse | Character instance, played by a user. Have their own usernames for stylistic/URL reasons
Starter | The primary unit of roleplay. A character's mun proposes the beginning of a scene.
Thread | The secondary unit of roleplay. Another character's mun accepts the proposal, transforming the proposed scene into a scene proper. More than one Thread can result from a given Starter because more than one character can accept the proposed scene (unless the starter says otherwise). Includes a notion of whose turn it is.
Post | The atomic unit of roleplay. A character speaks, or takes action. There's always one embedded in a starter.
ACL | An identifier used purely for many-to-many mappings to whitelisted or blacklisted muses/muns(/comms?). Can be copied from other ACLs but can't include them other than by "members of this comm".
Comm | A venue in which RP occurs. Has owners and members, both defined via ACLs.
Page | Arbitrary (static! no JS! no iframes!) content associable with a comm or muse.
Chat | An OOC conversation. Can be in the process of playing out a thread (in which case it's anchored to the relevant post) or in the context of a community.
Comment | Atom of a chat.
App | A character's mun proposes that character's membership in a comm. Some comms allow anyone to join.
Avatar | Little faces for muses.
Userpic | Little faces for muns.
Request | Request to change the terms of a thread. Most common case: another character arriving. All participants need to approve? Or just starter?
Dash | For users to look at their subscriptions. A user can have more than one dash. One of them is a default one.
Fact | Charahub bio entries.
Question | Charahub askblog feature. Controlled by muse-level ACLs.
Answer |  Charahub askblog feature.
Art | ...of characters. Charahub feature. Uploader can only tag a character if not blocked by their mun.


### Asymmetric

| Group A | has      | Group B |
|---------|----------|---------|
| Mun     | has      | ACL (B) |
| Mun     | has      | ACL (F) |
| Mun     | may have | Dash    |
| Mun     | may have | Userpic |
| Muse    | has      | Mun     |
| Muse    | may have | Avatar  |
| Starter | has      | Post    |
| Starter | may have | Comm    |
| Starter | may have | ACL (R) |
| Starter | may have | ACL (W) |
| Thread  | has      | Starter |
| Thread  | may have | ACL (R) |
| Thread  | may have | ACL (W) |
| Thread  | may have | Muse (C)|
| Post    | may have | Thread  |
| Post    | has      | Muse    |
| Post    | may have | Avatar  |
| Chat    | may have | Comm    |
| Chat    | may have | Post    |
| Chat    | may have | Muse    |
| Message | has      | Chat    |
| Message | has      | Mun     |
| Message | may have | Userpic |
| Page    | may have | Comm    |
| Page    | may have | Muse    |
| Comm    | may have | ACL (R) |
| Comm    | has      | ACL (M) |
| Comm    | has      | ACL (A) |
| App     | has      | Comm    |
| App     | has      | Muse    |
| App     | may have | Mun (A) |
| Avatar  | has      | Muse    |
| Userpic | has      | Mun     |
| Request | has      | Thread  |
| Request | may have | ACL (R) |
| Request | may have | ACL (W) |
| Dash    | has      | Mun     |
| Dash    | has      | ACL     |
| Fact    | has      | Muse    |
| Question| has      | Muse(R) |
| Question| may have | Mun (S) |
| Question| may have | Muse(S) |
| Question| may have | Post    |
| Art     | has      | Mun     |

### Many-to-many

| M       | N      |
|---------|--------|
| Muse    | Comm   |
| Starter | Tag    |
| Thread  | Tag    |
| Mun     | Tag (Filter)|
| Mun     | ACL (Preset)|
| ACL     | Mun    |
| ACL     | Muse   |
| ACL     | Comm   |
| Muse    | Muse (CR) |
| Art     | Muse   |
| Request | Mun (Y/N) |
| Art     | Tag    |
| Muse    | Tag?   |
