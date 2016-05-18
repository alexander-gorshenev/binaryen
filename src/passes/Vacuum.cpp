/*
 * Copyright 2016 WebAssembly Community Group participants
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

//
// Removes obviously unneeded code
//

#include <wasm.h>
#include <pass.h>
#include <ast_utils.h>
#include <wasm-builder.h>

namespace wasm {

struct Vacuum : public WalkerPass<PostWalker<Vacuum, Visitor<Vacuum>>> {
  bool isFunctionParallel() { return true; }

  std::vector<Expression*> expressionStack;

  bool isDead(Expression* curr, bool resultMayBeUsed) {
    if (curr->is<Nop>()) return true;
    // dead get_locals may be generated from coalesce-locals
    if (curr->is<GetLocal>() && (!resultMayBeUsed || !ExpressionAnalyzer::isResultUsed(expressionStack, getFunction()))) return true;
    // TODO: more dead code
    return false;
  }

  void visitBlock(Block *curr) {
    // compress out nops and other dead code
    int skip = 0;
    auto& list = curr->list;
    size_t size = list.size();
    bool needResize = false;
    for (size_t z = 0; z < size; z++) {
      if (isDead(list[z], z == size - 1)) {
        skip++;
        needResize = true;
      } else {
        if (skip > 0) {
          list[z - skip] = list[z];
        }
        // if this is an unconditional br, the rest is dead code
        Break* br = list[z - skip]->dynCast<Break>();
        Switch* sw = list[z - skip]->dynCast<Switch>();
        if ((br && !br->condition) || sw) {
          list.resize(z - skip + 1);
          needResize = false;
          break;
        }
      }
    }
    if (needResize) {
      list.resize(size - skip);
    }
    if (!curr->name.is()) {
      if (list.size() == 1) {
        replaceCurrent(list[0]);
      } else if (list.size() == 0) {
        ExpressionManipulator::nop(curr);
      }
    }
  }

  void visitIf(If* curr) {
    if (curr->ifFalse) {
      if (curr->ifFalse->is<Nop>()) {
        curr->ifFalse = nullptr;
      } else if (curr->ifTrue->is<Nop>()) {
        curr->ifTrue = curr->ifFalse;
        curr->ifFalse = nullptr;
        curr->condition = Builder(*getModule()).makeUnary(EqZInt32, curr->condition);
      }
    }
    if (!curr->ifFalse) {
      // no else
      if (curr->ifTrue->is<Nop>()) {
        // no nothing
        replaceCurrent(curr->condition);
      }
    }
  }

  static void visitPre(Vacuum* self, Expression** currp) {
    self->expressionStack.push_back(*currp);
  }

  static void visitPost(Vacuum* self, Expression** currp) {
    self->expressionStack.pop_back();
  }

  // override scan to add a pre and a post check task to all nodes
  static void scan(Vacuum* self, Expression** currp) {
    self->pushTask(visitPost, currp);

    WalkerPass<PostWalker<Vacuum, Visitor<Vacuum>>>::scan(self, currp);

    self->pushTask(visitPre, currp);
  }
};

static RegisterPass<Vacuum> registerPass("vacuum", "removes obviously unneeded code");

} // namespace wasm

