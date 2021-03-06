package com.bbn.parliament.jena.graph.index.temporal;

import java.io.File;

import com.bbn.parliament.jena.graph.index.IndexFactory.IndexFactoryHelper;
import com.bbn.parliament.jena.graph.index.Record;
import com.bbn.parliament.jena.graph.index.temporal.bdb.PersistentTemporalIndex;
import com.bbn.parliament.jena.graph.index.temporal.extent.TemporalExtent;
import com.bbn.parliament.jena.graph.index.temporal.extent.TemporalInstant;
import com.bbn.parliament.jena.graph.index.temporal.extent.TemporalInterval;
import com.bbn.parliament.jena.query.index.QueryableIndexTestMethods;
import com.hp.hpl.jena.graph.Graph;
import com.hp.hpl.jena.graph.Node;

public class TemporalIndexTestMethods extends QueryableIndexTestMethods<TemporalIndex, TemporalExtent> {
	private long creationTime = System.currentTimeMillis();

	@Override
	protected TemporalIndexFactory getIndexFactory() {
		return new TemporalIndexFactory();
	}

	@Override
	protected Record<TemporalExtent> createRecord(int seed) {
		Node n = Node.createURI("http://example.org/z" + seed);
		if (seed % 2 == 0) {
			TemporalInstant startTime = new TemporalInstant(creationTime + (100 * seed));
			TemporalInstant endTime = new TemporalInstant(creationTime + (100 * seed) + 1000);
			return Record.create(n, new TemporalInterval(startTime, endTime));
		} else {
			return Record.create(n, new TemporalInstant(creationTime + (100 * seed)));
		}
	}

	@Override
	protected void doSetup() {
	}

	@Override
	protected boolean checkDeleted(TemporalIndex index, Graph graph, Node graphName) {
		File indexDir = new File(IndexFactoryHelper.getIndexDirectory(graph, graphName),
			PersistentTemporalIndex.INDEX_SUB_DIR);
		return !indexDir.exists();
	}
}
