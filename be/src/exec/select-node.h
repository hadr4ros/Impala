// Copyright 2012 Cloudera Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.


#ifndef IMPALA_EXEC_SELECT_NODE_H
#define IMPALA_EXEC_SELECT_NODE_H

#include <boost/scoped_ptr.hpp>

#include "exec/exec-node.h"
#include "runtime/mem-pool.h"
#include <boost/scoped_ptr.hpp>

namespace impala {

class Tuple;
class TupleRow;

// Node that evaluates conjuncts and enforces a limit but otherwise passes along
// the rows pulled from its child unchanged.
class SelectNode : public ExecNode {
 public:
  SelectNode(ObjectPool* pool, const TPlanNode& tnode, const DescriptorTbl& descs);

  virtual Status Prepare(RuntimeState* state);
  virtual Status Open(RuntimeState* state);
  virtual Status GetNext(RuntimeState* state, RowBatch* row_batch, bool* eos);
  virtual Status Reset(RuntimeState* state);
  virtual void Close(RuntimeState* state);

 private:
  // current row batch of child
  boost::scoped_ptr<RowBatch> child_row_batch_;

  // index of current row in child_row_batch_
  int child_row_idx_;

  // true if last GetNext() call on child signalled eos
  bool child_eos_;

  // Copy rows from child_row_batch_ for which conjuncts_ evaluate to true to
  // output_batch, up to limit_.
  // Return true if limit was hit or output_batch should be returned, otherwise false.
  bool CopyRows(RowBatch* output_batch);
};

}

#endif
