# Libraries

| Name                     | Description |
|--------------------------|-------------|
| *libmippscoin_cli*         | RPC client functionality used by *mippscoin-cli* executable |
| *libmippscoin_common*      | Home for common functionality shared by different executables and libraries. Similar to *libmippscoin_util*, but higher-level (see [Dependencies](#dependencies)). |
| *libmippscoin_consensus*   | Stable, backwards-compatible consensus functionality used by *libmippscoin_node* and *libmippscoin_wallet* and also exposed as a [shared library](../shared-libraries.md). |
| *libmippscoinconsensus*    | Shared library build of static *libmippscoin_consensus* library |
| *libmippscoin_kernel*      | Consensus engine and support library used for validation by *libmippscoin_node* and also exposed as a [shared library](../shared-libraries.md). |
| *libmippscoinqt*           | GUI functionality used by *mippscoin-qt* and *mippscoin-gui* executables |
| *libmippscoin_ipc*         | IPC functionality used by *mippscoin-node*, *mippscoin-wallet*, *mippscoin-gui* executables to communicate when [`--enable-multiprocess`](multiprocess.md) is used. |
| *libmippscoin_node*        | P2P and RPC server functionality used by *mippscoind* and *mippscoin-qt* executables. |
| *libmippscoin_util*        | Home for common functionality shared by different executables and libraries. Similar to *libmippscoin_common*, but lower-level (see [Dependencies](#dependencies)). |
| *libmippscoin_wallet*      | Wallet functionality used by *mippscoind* and *mippscoin-wallet* executables. |
| *libmippscoin_wallet_tool* | Lower-level wallet functionality used by *mippscoin-wallet* executable. |
| *libmippscoin_zmq*         | [ZeroMQ](../zmq.md) functionality used by *mippscoind* and *mippscoin-qt* executables. |

## Conventions

- Most libraries are internal libraries and have APIs which are completely unstable! There are few or no restrictions on backwards compatibility or rules about external dependencies. Exceptions are *libmippscoin_consensus* and *libmippscoin_kernel* which have external interfaces documented at [../shared-libraries.md](../shared-libraries.md).

- Generally each library should have a corresponding source directory and namespace. Source code organization is a work in progress, so it is true that some namespaces are applied inconsistently, and if you look at [`libmippscoin_*_SOURCES`](../../src/Makefile.am) lists you can see that many libraries pull in files from outside their source directory. But when working with libraries, it is good to follow a consistent pattern like:

  - *libmippscoin_node* code lives in `src/node/` in the `node::` namespace
  - *libmippscoin_wallet* code lives in `src/wallet/` in the `wallet::` namespace
  - *libmippscoin_ipc* code lives in `src/ipc/` in the `ipc::` namespace
  - *libmippscoin_util* code lives in `src/util/` in the `util::` namespace
  - *libmippscoin_consensus* code lives in `src/consensus/` in the `Consensus::` namespace

## Dependencies

- Libraries should minimize what other libraries they depend on, and only reference symbols following the arrows shown in the dependency graph below:

<table><tr><td>

```mermaid

%%{ init : { "flowchart" : { "curve" : "basis" }}}%%

graph TD;

mippscoin-cli[mippscoin-cli]-->libmippscoin_cli;

mippscoind[mippscoind]-->libmippscoin_node;
mippscoind[mippscoind]-->libmippscoin_wallet;

mippscoin-qt[mippscoin-qt]-->libmippscoin_node;
mippscoin-qt[mippscoin-qt]-->libmippscoinqt;
mippscoin-qt[mippscoin-qt]-->libmippscoin_wallet;

mippscoin-wallet[mippscoin-wallet]-->libmippscoin_wallet;
mippscoin-wallet[mippscoin-wallet]-->libmippscoin_wallet_tool;

libmippscoin_cli-->libmippscoin_util;
libmippscoin_cli-->libmippscoin_common;

libmippscoin_common-->libmippscoin_consensus;
libmippscoin_common-->libmippscoin_util;

libmippscoin_kernel-->libmippscoin_consensus;
libmippscoin_kernel-->libmippscoin_util;

libmippscoin_node-->libmippscoin_consensus;
libmippscoin_node-->libmippscoin_kernel;
libmippscoin_node-->libmippscoin_common;
libmippscoin_node-->libmippscoin_util;

libmippscoinqt-->libmippscoin_common;
libmippscoinqt-->libmippscoin_util;

libmippscoin_wallet-->libmippscoin_common;
libmippscoin_wallet-->libmippscoin_util;

libmippscoin_wallet_tool-->libmippscoin_wallet;
libmippscoin_wallet_tool-->libmippscoin_util;

classDef bold stroke-width:2px, font-weight:bold, font-size: smaller;
class mippscoin-qt,mippscoind,mippscoin-cli,mippscoin-wallet bold
```
</td></tr><tr><td>

**Dependency graph**. Arrows show linker symbol dependencies. *Consensus* lib depends on nothing. *Util* lib is depended on by everything. *Kernel* lib depends only on consensus and util.

</td></tr></table>

- The graph shows what _linker symbols_ (functions and variables) from each library other libraries can call and reference directly, but it is not a call graph. For example, there is no arrow connecting *libmippscoin_wallet* and *libmippscoin_node* libraries, because these libraries are intended to be modular and not depend on each other's internal implementation details. But wallet code is still able to call node code indirectly through the `interfaces::Chain` abstract class in [`interfaces/chain.h`](../../src/interfaces/chain.h) and node code calls wallet code through the `interfaces::ChainClient` and `interfaces::Chain::Notifications` abstract classes in the same file. In general, defining abstract classes in [`src/interfaces/`](../../src/interfaces/) can be a convenient way of avoiding unwanted direct dependencies or circular dependencies between libraries.

- *libmippscoin_consensus* should be a standalone dependency that any library can depend on, and it should not depend on any other libraries itself.

- *libmippscoin_util* should also be a standalone dependency that any library can depend on, and it should not depend on other internal libraries.

- *libmippscoin_common* should serve a similar function as *libmippscoin_util* and be a place for miscellaneous code used by various daemon, GUI, and CLI applications and libraries to live. It should not depend on anything other than *libmippscoin_util* and *libmippscoin_consensus*. The boundary between _util_ and _common_ is a little fuzzy but historically _util_ has been used for more generic, lower-level things like parsing hex, and _common_ has been used for mippscoin-specific, higher-level things like parsing base58. The difference between util and common is mostly important because *libmippscoin_kernel* is not supposed to depend on *libmippscoin_common*, only *libmippscoin_util*. In general, if it is ever unclear whether it is better to add code to *util* or *common*, it is probably better to add it to *common* unless it is very generically useful or useful particularly to include in the kernel.


- *libmippscoin_kernel* should only depend on *libmippscoin_util* and *libmippscoin_consensus*.

- The only thing that should depend on *libmippscoin_kernel* internally should be *libmippscoin_node*. GUI and wallet libraries *libmippscoinqt* and *libmippscoin_wallet* in particular should not depend on *libmippscoin_kernel* and the unneeded functionality it would pull in, like block validation. To the extent that GUI and wallet code need scripting and signing functionality, they should be get able it from *libmippscoin_consensus*, *libmippscoin_common*, and *libmippscoin_util*, instead of *libmippscoin_kernel*.

- GUI, node, and wallet code internal implementations should all be independent of each other, and the *libmippscoinqt*, *libmippscoin_node*, *libmippscoin_wallet* libraries should never reference each other's symbols. They should only call each other through [`src/interfaces/`](`../../src/interfaces/`) abstract interfaces.

## Work in progress

- Validation code is moving from *libmippscoin_node* to *libmippscoin_kernel* as part of [The libmippscoinkernel Project #24303](https://github.com/mippscoin/mippscoin/issues/24303)
- Source code organization is discussed in general in [Library source code organization #15732](https://github.com/mippscoin/mippscoin/issues/15732)
