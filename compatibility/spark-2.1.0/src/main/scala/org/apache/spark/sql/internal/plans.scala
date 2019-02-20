/*
 * Copyright (c) 2018 SnappyData, Inc. All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you
 * may not use this file except in compliance with the License. You
 * may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or
 * implied. See the License for the specific language governing
 * permissions and limitations under the License. See accompanying
 * LICENSE file.
 */
package org.apache.spark.sql.internal

import io.snappydata.{HintName, QueryHint}

import org.apache.spark.sql.catalyst.expressions.{Attribute, AttributeReference}
import org.apache.spark.sql.catalyst.plans.logical.{BroadcastHint, InsertIntoTable, LogicalPlan, OverwriteOptions}
import org.apache.spark.sql.types.LongType


/**
 * Unlike Spark's InsertIntoTable this plan provides the count of rows
 * inserted as the output.
 */
final class Insert(
    table: LogicalPlan,
    partition: Map[String, Option[String]],
    child: LogicalPlan,
    overwrite: OverwriteOptions,
    ifNotExists: Boolean)
    extends InsertIntoTable(table, partition, child, overwrite, ifNotExists) {

  override def output: Seq[Attribute] = AttributeReference(
    "count", LongType)() :: Nil

  override def copy(table: LogicalPlan = table,
      partition: Map[String, Option[String]] = partition,
      child: LogicalPlan = child,
      overwrite: OverwriteOptions = overwrite,
      ifNotExists: Boolean = ifNotExists): Insert = {
    new Insert(table, partition, child, overwrite, ifNotExists)
  }
}

/**
 * An extended version of [[BroadcastHint]] to encapsulate any kind of hint rather
 * than just broadcast.
 */
class PlanWithHints(_child: LogicalPlan, override val allHints: Map[QueryHint.Type, HintName.Type])
    extends BroadcastHint(_child) with LogicalPlanWithHints {

  override def productArity: Int = 2

  override def productElement(n: Int): Any = n match {
    case 0 => child
    case 1 => allHints
  }
}