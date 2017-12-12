#include "../def.h"
#include "../containers/relation.h"
#include "../containers/bucket_index.h"



unsigned long long ForwardScanBased_PlaneSweep_Grouping_Bucketing_Rolled(Relation &R, Relation &S, const BucketIndex &BIR, const BucketIndex &BIS)
{
	auto r = R.begin();
	auto s = S.begin();
	auto lastR = R.end();
	auto lastS = S.end();
	unsigned long long result = 0;
	int cbucket_id, pbucket_id;
	Group GR, GS;
	
	
	while ((r < lastR) && (s < lastS))
	{
		if (*r < *s)
		{
			//step 1: gather group for R
			while ((r < lastR) && (r->start < s->start))
			{
				GR.emplace_back(r->start, r->end);
				r++;
			}
			
			//sort group by end point
			GR.sortByEnd();
			
			//step 2: run internal loop
			auto pivot = s;
			auto lastGR = GR.end();
			for (auto curr = GR.begin(); curr != lastGR; curr++)
			{
				if (pivot == lastS)
					break;
				if (curr->end < S.minStart)
					continue;
				
				cbucket_id = ceil((double)(curr->end-S.minStart)/BIS.bucket_range);
				if (cbucket_id >= BIS.numBuckets)
					cbucket_id = BIS.numBuckets-1;
				pbucket_id = ceil((double)(pivot->end-S.minStart)/BIS.bucket_range);
				if (pbucket_id >= BIS.numBuckets)
					pbucket_id = BIS.numBuckets-1;
				
				if (cbucket_id > pbucket_id)
				{
					auto last = BIS[cbucket_id-1].last;
					while (pivot != last)
					{
						for (auto k = curr; k != lastGR; k++)
						{
							result += k->start ^ pivot->start;
						}
						
						pivot++;
					}
				}
				
				// Sweep the last bucket.
				auto last = BIS[cbucket_id].last;
				while ((pivot != last) && (curr->end >= pivot->start))
				{
					for (auto k = curr; k != lastGR; k++)
					{
						result += k->start ^ pivot->start;
					}
					pivot++;
				}
			}
			GR.clear();
		}
		else
		{
			//step 1: gather group for S
			while ((s < lastS) && (r->start >= s->start))
			{
				GS.emplace_back(s->start, s->end);
				s++;
			}
			
			//sort group by end point
			GS.sortByEnd();
			
			
			//step 2: run internal loop
			auto pivot = r;
			auto lastGS = GS.end();
			for (auto curs = GS.begin(); curs != lastGS; curs++)
			{
				if (pivot == lastR)
					break;
				if (curs->end < R.minStart)
					continue;
				
				cbucket_id = ceil((double)(curs->end-R.minStart)/BIR.bucket_range);
				if (cbucket_id >= BIR.numBuckets)
					cbucket_id = BIR.numBuckets-1;
				pbucket_id = ceil((double)(pivot->end-R.minStart)/BIR.bucket_range);
				if (pbucket_id >= BIR.numBuckets)
					pbucket_id = BIR.numBuckets-1;
				
				if (cbucket_id > pbucket_id)
				{
					auto last = BIR[cbucket_id-1].last;
					while (pivot != last)
					{
						for (auto k = curs; k != lastGS; k++)
						{
							result += pivot->start ^ k->start;
						}
						pivot++;
					}
				}
				
				// Sweep the last bucket.
				auto last = BIR[cbucket_id].last;
				while ((pivot != last) && (curs->end >= pivot->start))
				{
					for (auto k = curs; k != lastGS; k++)
					{
						result += pivot->start ^ k->start;
					}
					pivot++;
				}
			}
			GS.clear();
		}
	}
	
	
	return result;
}


unsigned long long ForwardScanBased_PlaneSweep_Grouping_Bucketing_Unrolled(Relation &R, Relation &S, const BucketIndex &BIR, const BucketIndex &BIS)
{
	auto r = R.begin();
	auto s = S.begin();
	auto lastR = R.end();
	auto lastS = S.end();
	unsigned long long result = 0;
	int cbucket_id, tmp_res, pbucket_id;
	Group GR, GS;
	
	
	while ((r < lastR) && (s < lastS))
	{
		if (*r < *s)
		{
			//step 1: gather group for R
			while ((r < lastR) && (r->start < s->start))
			{
				GR.emplace_back(r->start, r->end);
				r++;
			}
			
			//sort group by end point
			GR.sortByEnd();
			
			//step 2: run internal loop
			auto pivot = s;
			auto lastGR = GR.end();
			for (auto curr = GR.begin(); curr != lastGR; curr++)
			{
				auto bufferSize = (lastGR-curr);
				
				if (pivot == lastS)
					break;
				if (curr->end < S.minStart)
					continue;
				
				cbucket_id = ceil((double)(curr->end-S.minStart)/BIS.bucket_range);
				if (cbucket_id >= BIS.numBuckets)
					cbucket_id = BIS.numBuckets-1;
				pbucket_id = ceil((double)(pivot->end-S.minStart)/BIS.bucket_range);
				if (pbucket_id >= BIS.numBuckets)
					pbucket_id = BIS.numBuckets-1;
				
				if (cbucket_id > pbucket_id)
				{
					auto last = BIS[cbucket_id-1].last;
					switch (bufferSize)
					{
						case 1:
							while (last-pivot >= 32)
							{
								result += curr->start ^ (pivot+0)->start;
								result += curr->start ^ (pivot+1)->start;
								result += curr->start ^ (pivot+2)->start;
								result += curr->start ^ (pivot+3)->start;
								result += curr->start ^ (pivot+4)->start;
								result += curr->start ^ (pivot+5)->start;
								result += curr->start ^ (pivot+6)->start;
								result += curr->start ^ (pivot+7)->start;
								result += curr->start ^ (pivot+8)->start;
								result += curr->start ^ (pivot+9)->start;
								result += curr->start ^ (pivot+10)->start;
								result += curr->start ^ (pivot+11)->start;
								result += curr->start ^ (pivot+12)->start;
								result += curr->start ^ (pivot+13)->start;
								result += curr->start ^ (pivot+14)->start;
								result += curr->start ^ (pivot+15)->start;
								result += curr->start ^ (pivot+16)->start;
								result += curr->start ^ (pivot+17)->start;
								result += curr->start ^ (pivot+18)->start;
								result += curr->start ^ (pivot+19)->start;
								result += curr->start ^ (pivot+20)->start;
								result += curr->start ^ (pivot+21)->start;
								result += curr->start ^ (pivot+22)->start;
								result += curr->start ^ (pivot+23)->start;
								result += curr->start ^ (pivot+24)->start;
								result += curr->start ^ (pivot+25)->start;
								result += curr->start ^ (pivot+26)->start;
								result += curr->start ^ (pivot+27)->start;
								result += curr->start ^ (pivot+28)->start;
								result += curr->start ^ (pivot+29)->start;
								result += curr->start ^ (pivot+30)->start;
								result += curr->start ^ (pivot+31)->start;
								pivot += 32;
							}
							
							while (pivot < last)
							{
								result += curr->start ^ pivot->start;
								pivot++;
							}
							break;
							
						case 2:
							while (last-pivot >= 16)
							{
								result += (curr+0)->start ^ (pivot+0)->start;
								result += (curr+1)->start ^ (pivot+0)->start;
								result += (curr+0)->start ^ (pivot+1)->start;
								result += (curr+1)->start ^ (pivot+1)->start;
								result += (curr+0)->start ^ (pivot+2)->start;
								result += (curr+1)->start ^ (pivot+2)->start;
								result += (curr+0)->start ^ (pivot+3)->start;
								result += (curr+1)->start ^ (pivot+3)->start;
								result += (curr+0)->start ^ (pivot+4)->start;
								result += (curr+1)->start ^ (pivot+4)->start;
								result += (curr+0)->start ^ (pivot+5)->start;
								result += (curr+1)->start ^ (pivot+5)->start;
								result += (curr+0)->start ^ (pivot+6)->start;
								result += (curr+1)->start ^ (pivot+6)->start;
								result += (curr+0)->start ^ (pivot+7)->start;
								result += (curr+1)->start ^ (pivot+7)->start;
								result += (curr+0)->start ^ (pivot+8)->start;
								result += (curr+1)->start ^ (pivot+8)->start;
								result += (curr+0)->start ^ (pivot+9)->start;
								result += (curr+1)->start ^ (pivot+9)->start;
								result += (curr+0)->start ^ (pivot+10)->start;
								result += (curr+1)->start ^ (pivot+10)->start;
								result += (curr+0)->start ^ (pivot+11)->start;
								result += (curr+1)->start ^ (pivot+11)->start;
								result += (curr+0)->start ^ (pivot+12)->start;
								result += (curr+1)->start ^ (pivot+12)->start;
								result += (curr+0)->start ^ (pivot+13)->start;
								result += (curr+1)->start ^ (pivot+13)->start;
								result += (curr+0)->start ^ (pivot+14)->start;
								result += (curr+1)->start ^ (pivot+14)->start;
								result += (curr+0)->start ^ (pivot+15)->start;
								result += (curr+1)->start ^ (pivot+15)->start;
								pivot += 16;
							}
							
							while (pivot < last)
							{
								result += (curr+0)->start ^ pivot->start;
								result += (curr+1)->start ^ pivot->start;
								pivot++;
							}
							break;
							
						case 3:
							while (last-pivot >= 8)
							{
								result += (curr+0)->start ^ (pivot+0)->start;
								result += (curr+1)->start ^ (pivot+0)->start;
								result += (curr+2)->start ^ (pivot+0)->start;
								result += (curr+0)->start ^ (pivot+1)->start;
								result += (curr+1)->start ^ (pivot+1)->start;
								result += (curr+2)->start ^ (pivot+1)->start;
								result += (curr+0)->start ^ (pivot+2)->start;
								result += (curr+1)->start ^ (pivot+2)->start;
								result += (curr+2)->start ^ (pivot+2)->start;
								result += (curr+0)->start ^ (pivot+3)->start;
								result += (curr+1)->start ^ (pivot+3)->start;
								result += (curr+2)->start ^ (pivot+3)->start;
								result += (curr+0)->start ^ (pivot+4)->start;
								result += (curr+1)->start ^ (pivot+4)->start;
								result += (curr+2)->start ^ (pivot+4)->start;
								result += (curr+0)->start ^ (pivot+5)->start;
								result += (curr+1)->start ^ (pivot+5)->start;
								result += (curr+2)->start ^ (pivot+5)->start;
								result += (curr+0)->start ^ (pivot+6)->start;
								result += (curr+1)->start ^ (pivot+6)->start;
								result += (curr+2)->start ^ (pivot+6)->start;
								result += (curr+0)->start ^ (pivot+7)->start;
								result += (curr+1)->start ^ (pivot+7)->start;
								result += (curr+2)->start ^ (pivot+7)->start;
								pivot += 8;
							}
							
							while (pivot < last)
							{
								result += (curr+0)->start ^ pivot->start;
								result += (curr+1)->start ^ pivot->start;
								result += (curr+2)->start ^ pivot->start;
								pivot++;
							}
							break;
							
						case 4:
							while (last-pivot >= 8)
							{
								result += (curr+0)->start ^ (pivot+0)->start;
								result += (curr+1)->start ^ (pivot+0)->start;
								result += (curr+2)->start ^ (pivot+0)->start;
								result += (curr+3)->start ^ (pivot+0)->start;
								result += (curr+0)->start ^ (pivot+1)->start;
								result += (curr+1)->start ^ (pivot+1)->start;
								result += (curr+2)->start ^ (pivot+1)->start;
								result += (curr+3)->start ^ (pivot+1)->start;
								result += (curr+0)->start ^ (pivot+2)->start;
								result += (curr+1)->start ^ (pivot+2)->start;
								result += (curr+2)->start ^ (pivot+2)->start;
								result += (curr+3)->start ^ (pivot+2)->start;
								result += (curr+0)->start ^ (pivot+3)->start;
								result += (curr+1)->start ^ (pivot+3)->start;
								result += (curr+2)->start ^ (pivot+3)->start;
								result += (curr+3)->start ^ (pivot+3)->start;
								result += (curr+0)->start ^ (pivot+4)->start;
								result += (curr+1)->start ^ (pivot+4)->start;
								result += (curr+2)->start ^ (pivot+4)->start;
								result += (curr+3)->start ^ (pivot+4)->start;
								result += (curr+0)->start ^ (pivot+5)->start;
								result += (curr+1)->start ^ (pivot+5)->start;
								result += (curr+2)->start ^ (pivot+5)->start;
								result += (curr+3)->start ^ (pivot+5)->start;
								result += (curr+0)->start ^ (pivot+6)->start;
								result += (curr+1)->start ^ (pivot+6)->start;
								result += (curr+2)->start ^ (pivot+6)->start;
								result += (curr+3)->start ^ (pivot+6)->start;
								result += (curr+0)->start ^ (pivot+7)->start;
								result += (curr+1)->start ^ (pivot+7)->start;
								result += (curr+2)->start ^ (pivot+7)->start;
								result += (curr+3)->start ^ (pivot+7)->start;
								pivot += 8;
							}
							
							while (pivot < last)
							{
								result += (curr+0)->start ^ pivot->start;
								result += (curr+1)->start ^ pivot->start;
								result += (curr+2)->start ^ pivot->start;
								result += (curr+3)->start ^ pivot->start;
								pivot++;
							}
							break;
							
						case 5:
							while (pivot < last)
							{
								result += (curr+0)->start ^ pivot->start;
								result += (curr+1)->start ^ pivot->start;
								result += (curr+2)->start ^ pivot->start;
								result += (curr+3)->start ^ pivot->start;
								result += (curr+4)->start ^ pivot->start;
								pivot++;
							}
							break;
							
						case 6:
							while (pivot < last)
							{
								result += (curr+0)->start ^ pivot->start;
								result += (curr+1)->start ^ pivot->start;
								result += (curr+2)->start ^ pivot->start;
								result += (curr+3)->start ^ pivot->start;
								result += (curr+4)->start ^ pivot->start;
								result += (curr+5)->start ^ pivot->start;
								pivot++;
							}
							break;
							
						case 7:
							while (pivot < last)
							{
								result += (curr+0)->start ^ pivot->start;
								result += (curr+1)->start ^ pivot->start;
								result += (curr+2)->start ^ pivot->start;
								result += (curr+3)->start ^ pivot->start;
								result += (curr+4)->start ^ pivot->start;
								result += (curr+5)->start ^ pivot->start;
								result += (curr+6)->start ^ pivot->start;
								pivot++;
							}
							break;
							
						case 8:
							while (pivot < last)
							{
								result += (curr+0)->start ^ pivot->start;
								result += (curr+1)->start ^ pivot->start;
								result += (curr+2)->start ^ pivot->start;
								result += (curr+3)->start ^ pivot->start;
								result += (curr+4)->start ^ pivot->start;
								result += (curr+5)->start ^ pivot->start;
								result += (curr+6)->start ^ pivot->start;
								result += (curr+7)->start ^ pivot->start;
								pivot++;
							}
							break;
							
						case 9:
							while (pivot < last)
							{
								result += (curr+0)->start ^ pivot->start;
								result += (curr+1)->start ^ pivot->start;
								result += (curr+2)->start ^ pivot->start;
								result += (curr+3)->start ^ pivot->start;
								result += (curr+4)->start ^ pivot->start;
								result += (curr+5)->start ^ pivot->start;
								result += (curr+6)->start ^ pivot->start;
								result += (curr+7)->start ^ pivot->start;
								result += (curr+8)->start ^ pivot->start;
								pivot++;
							}
							break;
							
						case 10:
							while (pivot < last)
							{
								result += (curr+0)->start ^ pivot->start;
								result += (curr+1)->start ^ pivot->start;
								result += (curr+2)->start ^ pivot->start;
								result += (curr+3)->start ^ pivot->start;
								result += (curr+4)->start ^ pivot->start;
								result += (curr+5)->start ^ pivot->start;
								result += (curr+6)->start ^ pivot->start;
								result += (curr+7)->start ^ pivot->start;
								result += (curr+8)->start ^ pivot->start;
								result += (curr+9)->start ^ pivot->start;
								pivot++;
							}
							break;
							
						case 11:
							while (pivot < last)
							{
								result += (curr+0)->start ^ pivot->start;
								result += (curr+1)->start ^ pivot->start;
								result += (curr+2)->start ^ pivot->start;
								result += (curr+3)->start ^ pivot->start;
								result += (curr+4)->start ^ pivot->start;
								result += (curr+5)->start ^ pivot->start;
								result += (curr+6)->start ^ pivot->start;
								result += (curr+7)->start ^ pivot->start;
								result += (curr+8)->start ^ pivot->start;
								result += (curr+9)->start ^ pivot->start;
								result += (curr+10)->start ^ pivot->start;
								pivot++;
							}
							break;
							
						case 12:
							while (pivot < last)
							{
								result += (curr+0)->start ^ pivot->start;
								result += (curr+1)->start ^ pivot->start;
								result += (curr+2)->start ^ pivot->start;
								result += (curr+3)->start ^ pivot->start;
								result += (curr+4)->start ^ pivot->start;
								result += (curr+5)->start ^ pivot->start;
								result += (curr+6)->start ^ pivot->start;
								result += (curr+7)->start ^ pivot->start;
								result += (curr+8)->start ^ pivot->start;
								result += (curr+9)->start ^ pivot->start;
								result += (curr+10)->start ^ pivot->start;
								result += (curr+11)->start ^ pivot->start;
								pivot++;
							}
							break;
							
						case 13:
							while (pivot < last)
							{
								result += (curr+0)->start ^ pivot->start;
								result += (curr+1)->start ^ pivot->start;
								result += (curr+2)->start ^ pivot->start;
								result += (curr+3)->start ^ pivot->start;
								result += (curr+4)->start ^ pivot->start;
								result += (curr+5)->start ^ pivot->start;
								result += (curr+6)->start ^ pivot->start;
								result += (curr+7)->start ^ pivot->start;
								result += (curr+8)->start ^ pivot->start;
								result += (curr+9)->start ^ pivot->start;
								result += (curr+10)->start ^ pivot->start;
								result += (curr+11)->start ^ pivot->start;
								result += (curr+12)->start ^ pivot->start;
								pivot++;
							}
							break;
							
						case 14:
							while (pivot < last)
							{
								result += (curr+0)->start ^ pivot->start;
								result += (curr+1)->start ^ pivot->start;
								result += (curr+2)->start ^ pivot->start;
								result += (curr+3)->start ^ pivot->start;
								result += (curr+4)->start ^ pivot->start;
								result += (curr+5)->start ^ pivot->start;
								result += (curr+6)->start ^ pivot->start;
								result += (curr+7)->start ^ pivot->start;
								result += (curr+8)->start ^ pivot->start;
								result += (curr+9)->start ^ pivot->start;
								result += (curr+10)->start ^ pivot->start;
								result += (curr+11)->start ^ pivot->start;
								result += (curr+12)->start ^ pivot->start;
								result += (curr+13)->start ^ pivot->start;
								pivot++;
							}
							break;
							
						case 15:
							while (pivot < last)
							{
								result += (curr+0)->start ^ pivot->start;
								result += (curr+1)->start ^ pivot->start;
								result += (curr+2)->start ^ pivot->start;
								result += (curr+3)->start ^ pivot->start;
								result += (curr+4)->start ^ pivot->start;
								result += (curr+5)->start ^ pivot->start;
								result += (curr+6)->start ^ pivot->start;
								result += (curr+7)->start ^ pivot->start;
								result += (curr+8)->start ^ pivot->start;
								result += (curr+9)->start ^ pivot->start;
								result += (curr+10)->start ^ pivot->start;
								result += (curr+11)->start ^ pivot->start;
								result += (curr+12)->start ^ pivot->start;
								result += (curr+13)->start ^ pivot->start;
								result += (curr+14)->start ^ pivot->start;
								pivot++;
							}
							break;
							
						case 16:
							while (pivot < last)
							{
								result += (curr+0)->start ^ pivot->start;
								result += (curr+1)->start ^ pivot->start;
								result += (curr+2)->start ^ pivot->start;
								result += (curr+3)->start ^ pivot->start;
								result += (curr+4)->start ^ pivot->start;
								result += (curr+5)->start ^ pivot->start;
								result += (curr+6)->start ^ pivot->start;
								result += (curr+7)->start ^ pivot->start;
								result += (curr+8)->start ^ pivot->start;
								result += (curr+9)->start ^ pivot->start;
								result += (curr+10)->start ^ pivot->start;
								result += (curr+11)->start ^ pivot->start;
								result += (curr+12)->start ^ pivot->start;
								result += (curr+13)->start ^ pivot->start;
								result += (curr+14)->start ^ pivot->start;
								result += (curr+15)->start ^ pivot->start;
								pivot++;
							}
							break;
							
						case 17:
							while (pivot < last)
							{
								result += (curr+0)->start ^ pivot->start;
								result += (curr+1)->start ^ pivot->start;
								result += (curr+2)->start ^ pivot->start;
								result += (curr+3)->start ^ pivot->start;
								result += (curr+4)->start ^ pivot->start;
								result += (curr+5)->start ^ pivot->start;
								result += (curr+6)->start ^ pivot->start;
								result += (curr+7)->start ^ pivot->start;
								result += (curr+8)->start ^ pivot->start;
								result += (curr+9)->start ^ pivot->start;
								result += (curr+10)->start ^ pivot->start;
								result += (curr+11)->start ^ pivot->start;
								result += (curr+12)->start ^ pivot->start;
								result += (curr+13)->start ^ pivot->start;
								result += (curr+14)->start ^ pivot->start;
								result += (curr+15)->start ^ pivot->start;
								result += (curr+16)->start ^ pivot->start;
								pivot++;
							}
							break;
							
						case 18:
							while (pivot < last)
							{
								result += (curr+0)->start ^ pivot->start;
								result += (curr+1)->start ^ pivot->start;
								result += (curr+2)->start ^ pivot->start;
								result += (curr+3)->start ^ pivot->start;
								result += (curr+4)->start ^ pivot->start;
								result += (curr+5)->start ^ pivot->start;
								result += (curr+6)->start ^ pivot->start;
								result += (curr+7)->start ^ pivot->start;
								result += (curr+8)->start ^ pivot->start;
								result += (curr+9)->start ^ pivot->start;
								result += (curr+10)->start ^ pivot->start;
								result += (curr+11)->start ^ pivot->start;
								result += (curr+12)->start ^ pivot->start;
								result += (curr+13)->start ^ pivot->start;
								result += (curr+14)->start ^ pivot->start;
								result += (curr+15)->start ^ pivot->start;
								result += (curr+16)->start ^ pivot->start;
								result += (curr+17)->start ^ pivot->start;
								pivot++;
							}
							break;
							
						case 19:
							while (pivot < last)
							{
								result += (curr+0)->start ^ pivot->start;
								result += (curr+1)->start ^ pivot->start;
								result += (curr+2)->start ^ pivot->start;
								result += (curr+3)->start ^ pivot->start;
								result += (curr+4)->start ^ pivot->start;
								result += (curr+5)->start ^ pivot->start;
								result += (curr+6)->start ^ pivot->start;
								result += (curr+7)->start ^ pivot->start;
								result += (curr+8)->start ^ pivot->start;
								result += (curr+9)->start ^ pivot->start;
								result += (curr+10)->start ^ pivot->start;
								result += (curr+11)->start ^ pivot->start;
								result += (curr+12)->start ^ pivot->start;
								result += (curr+13)->start ^ pivot->start;
								result += (curr+14)->start ^ pivot->start;
								result += (curr+15)->start ^ pivot->start;
								result += (curr+16)->start ^ pivot->start;
								result += (curr+17)->start ^ pivot->start;
								result += (curr+18)->start ^ pivot->start;
								pivot++;
							}
							break;
							
						case 20:
							while (pivot < last)
							{
								result += (curr+0)->start ^ pivot->start;
								result += (curr+1)->start ^ pivot->start;
								result += (curr+2)->start ^ pivot->start;
								result += (curr+3)->start ^ pivot->start;
								result += (curr+4)->start ^ pivot->start;
								result += (curr+5)->start ^ pivot->start;
								result += (curr+6)->start ^ pivot->start;
								result += (curr+7)->start ^ pivot->start;
								result += (curr+8)->start ^ pivot->start;
								result += (curr+9)->start ^ pivot->start;
								result += (curr+10)->start ^ pivot->start;
								result += (curr+11)->start ^ pivot->start;
								result += (curr+12)->start ^ pivot->start;
								result += (curr+13)->start ^ pivot->start;
								result += (curr+14)->start ^ pivot->start;
								result += (curr+15)->start ^ pivot->start;
								result += (curr+16)->start ^ pivot->start;
								result += (curr+17)->start ^ pivot->start;
								result += (curr+18)->start ^ pivot->start;
								result += (curr+19)->start ^ pivot->start;
								pivot++;
							}
							break;
							
						case 21:
							while (pivot < last)
							{
								result += (curr+0)->start ^ pivot->start;
								result += (curr+1)->start ^ pivot->start;
								result += (curr+2)->start ^ pivot->start;
								result += (curr+3)->start ^ pivot->start;
								result += (curr+4)->start ^ pivot->start;
								result += (curr+5)->start ^ pivot->start;
								result += (curr+6)->start ^ pivot->start;
								result += (curr+7)->start ^ pivot->start;
								result += (curr+8)->start ^ pivot->start;
								result += (curr+9)->start ^ pivot->start;
								result += (curr+10)->start ^ pivot->start;
								result += (curr+11)->start ^ pivot->start;
								result += (curr+12)->start ^ pivot->start;
								result += (curr+13)->start ^ pivot->start;
								result += (curr+14)->start ^ pivot->start;
								result += (curr+15)->start ^ pivot->start;
								result += (curr+16)->start ^ pivot->start;
								result += (curr+17)->start ^ pivot->start;
								result += (curr+18)->start ^ pivot->start;
								result += (curr+19)->start ^ pivot->start;
								result += (curr+20)->start ^ pivot->start;
								pivot++;
							}
							break;
							
						case 22:
							while (pivot < last)
							{
								result += (curr+0)->start ^ pivot->start;
								result += (curr+1)->start ^ pivot->start;
								result += (curr+2)->start ^ pivot->start;
								result += (curr+3)->start ^ pivot->start;
								result += (curr+4)->start ^ pivot->start;
								result += (curr+5)->start ^ pivot->start;
								result += (curr+6)->start ^ pivot->start;
								result += (curr+7)->start ^ pivot->start;
								result += (curr+8)->start ^ pivot->start;
								result += (curr+9)->start ^ pivot->start;
								result += (curr+10)->start ^ pivot->start;
								result += (curr+11)->start ^ pivot->start;
								result += (curr+12)->start ^ pivot->start;
								result += (curr+13)->start ^ pivot->start;
								result += (curr+14)->start ^ pivot->start;
								result += (curr+15)->start ^ pivot->start;
								result += (curr+16)->start ^ pivot->start;
								result += (curr+17)->start ^ pivot->start;
								result += (curr+18)->start ^ pivot->start;
								result += (curr+19)->start ^ pivot->start;
								result += (curr+20)->start ^ pivot->start;
								result += (curr+21)->start ^ pivot->start;
								pivot++;
							}
							break;
							
						case 23:
							while (pivot < last)
							{
								result += (curr+0)->start ^ pivot->start;
								result += (curr+1)->start ^ pivot->start;
								result += (curr+2)->start ^ pivot->start;
								result += (curr+3)->start ^ pivot->start;
								result += (curr+4)->start ^ pivot->start;
								result += (curr+5)->start ^ pivot->start;
								result += (curr+6)->start ^ pivot->start;
								result += (curr+7)->start ^ pivot->start;
								result += (curr+8)->start ^ pivot->start;
								result += (curr+9)->start ^ pivot->start;
								result += (curr+10)->start ^ pivot->start;
								result += (curr+11)->start ^ pivot->start;
								result += (curr+12)->start ^ pivot->start;
								result += (curr+13)->start ^ pivot->start;
								result += (curr+14)->start ^ pivot->start;
								result += (curr+15)->start ^ pivot->start;
								result += (curr+16)->start ^ pivot->start;
								result += (curr+17)->start ^ pivot->start;
								result += (curr+18)->start ^ pivot->start;
								result += (curr+19)->start ^ pivot->start;
								result += (curr+20)->start ^ pivot->start;
								result += (curr+21)->start ^ pivot->start;
								result += (curr+22)->start ^ pivot->start;
								pivot++;
							}
							break;
							
						case 24:
							while (pivot < last)
							{
								result += (curr+0)->start ^ pivot->start;
								result += (curr+1)->start ^ pivot->start;
								result += (curr+2)->start ^ pivot->start;
								result += (curr+3)->start ^ pivot->start;
								result += (curr+4)->start ^ pivot->start;
								result += (curr+5)->start ^ pivot->start;
								result += (curr+6)->start ^ pivot->start;
								result += (curr+7)->start ^ pivot->start;
								result += (curr+8)->start ^ pivot->start;
								result += (curr+9)->start ^ pivot->start;
								result += (curr+10)->start ^ pivot->start;
								result += (curr+11)->start ^ pivot->start;
								result += (curr+12)->start ^ pivot->start;
								result += (curr+13)->start ^ pivot->start;
								result += (curr+14)->start ^ pivot->start;
								result += (curr+15)->start ^ pivot->start;
								result += (curr+16)->start ^ pivot->start;
								result += (curr+17)->start ^ pivot->start;
								result += (curr+18)->start ^ pivot->start;
								result += (curr+19)->start ^ pivot->start;
								result += (curr+20)->start ^ pivot->start;
								result += (curr+21)->start ^ pivot->start;
								result += (curr+22)->start ^ pivot->start;
								result += (curr+23)->start ^ pivot->start;
								pivot++;
							}
							break;
							
						case 25:
							while (pivot < last)
							{
								result += (curr+0)->start ^ pivot->start;
								result += (curr+1)->start ^ pivot->start;
								result += (curr+2)->start ^ pivot->start;
								result += (curr+3)->start ^ pivot->start;
								result += (curr+4)->start ^ pivot->start;
								result += (curr+5)->start ^ pivot->start;
								result += (curr+6)->start ^ pivot->start;
								result += (curr+7)->start ^ pivot->start;
								result += (curr+8)->start ^ pivot->start;
								result += (curr+9)->start ^ pivot->start;
								result += (curr+10)->start ^ pivot->start;
								result += (curr+11)->start ^ pivot->start;
								result += (curr+12)->start ^ pivot->start;
								result += (curr+13)->start ^ pivot->start;
								result += (curr+14)->start ^ pivot->start;
								result += (curr+15)->start ^ pivot->start;
								result += (curr+16)->start ^ pivot->start;
								result += (curr+17)->start ^ pivot->start;
								result += (curr+18)->start ^ pivot->start;
								result += (curr+19)->start ^ pivot->start;
								result += (curr+20)->start ^ pivot->start;
								result += (curr+21)->start ^ pivot->start;
								result += (curr+22)->start ^ pivot->start;
								result += (curr+23)->start ^ pivot->start;
								result += (curr+24)->start ^ pivot->start;
								pivot++;
							}
							break;
							
						case 26:
							while (pivot < last)
							{
								result += (curr+0)->start ^ pivot->start;
								result += (curr+1)->start ^ pivot->start;
								result += (curr+2)->start ^ pivot->start;
								result += (curr+3)->start ^ pivot->start;
								result += (curr+4)->start ^ pivot->start;
								result += (curr+5)->start ^ pivot->start;
								result += (curr+6)->start ^ pivot->start;
								result += (curr+7)->start ^ pivot->start;
								result += (curr+8)->start ^ pivot->start;
								result += (curr+9)->start ^ pivot->start;
								result += (curr+10)->start ^ pivot->start;
								result += (curr+11)->start ^ pivot->start;
								result += (curr+12)->start ^ pivot->start;
								result += (curr+13)->start ^ pivot->start;
								result += (curr+14)->start ^ pivot->start;
								result += (curr+15)->start ^ pivot->start;
								result += (curr+16)->start ^ pivot->start;
								result += (curr+17)->start ^ pivot->start;
								result += (curr+18)->start ^ pivot->start;
								result += (curr+19)->start ^ pivot->start;
								result += (curr+20)->start ^ pivot->start;
								result += (curr+21)->start ^ pivot->start;
								result += (curr+22)->start ^ pivot->start;
								result += (curr+23)->start ^ pivot->start;
								result += (curr+24)->start ^ pivot->start;
								result += (curr+25)->start ^ pivot->start;
								pivot++;
							}
							break;
							
						case 27:
							while (pivot < last)
							{
								result += (curr+0)->start ^ pivot->start;
								result += (curr+1)->start ^ pivot->start;
								result += (curr+2)->start ^ pivot->start;
								result += (curr+3)->start ^ pivot->start;
								result += (curr+4)->start ^ pivot->start;
								result += (curr+5)->start ^ pivot->start;
								result += (curr+6)->start ^ pivot->start;
								result += (curr+7)->start ^ pivot->start;
								result += (curr+8)->start ^ pivot->start;
								result += (curr+9)->start ^ pivot->start;
								result += (curr+10)->start ^ pivot->start;
								result += (curr+11)->start ^ pivot->start;
								result += (curr+12)->start ^ pivot->start;
								result += (curr+13)->start ^ pivot->start;
								result += (curr+14)->start ^ pivot->start;
								result += (curr+15)->start ^ pivot->start;
								result += (curr+16)->start ^ pivot->start;
								result += (curr+17)->start ^ pivot->start;
								result += (curr+18)->start ^ pivot->start;
								result += (curr+19)->start ^ pivot->start;
								result += (curr+20)->start ^ pivot->start;
								result += (curr+21)->start ^ pivot->start;
								result += (curr+22)->start ^ pivot->start;
								result += (curr+23)->start ^ pivot->start;
								result += (curr+24)->start ^ pivot->start;
								result += (curr+25)->start ^ pivot->start;
								result += (curr+26)->start ^ pivot->start;
								pivot++;
							}
							break;
							
						case 28:
							while (pivot < last)
							{
								result += (curr+0)->start ^ pivot->start;
								result += (curr+1)->start ^ pivot->start;
								result += (curr+2)->start ^ pivot->start;
								result += (curr+3)->start ^ pivot->start;
								result += (curr+4)->start ^ pivot->start;
								result += (curr+5)->start ^ pivot->start;
								result += (curr+6)->start ^ pivot->start;
								result += (curr+7)->start ^ pivot->start;
								result += (curr+8)->start ^ pivot->start;
								result += (curr+9)->start ^ pivot->start;
								result += (curr+10)->start ^ pivot->start;
								result += (curr+11)->start ^ pivot->start;
								result += (curr+12)->start ^ pivot->start;
								result += (curr+13)->start ^ pivot->start;
								result += (curr+14)->start ^ pivot->start;
								result += (curr+15)->start ^ pivot->start;
								result += (curr+16)->start ^ pivot->start;
								result += (curr+17)->start ^ pivot->start;
								result += (curr+18)->start ^ pivot->start;
								result += (curr+19)->start ^ pivot->start;
								result += (curr+20)->start ^ pivot->start;
								result += (curr+21)->start ^ pivot->start;
								result += (curr+22)->start ^ pivot->start;
								result += (curr+23)->start ^ pivot->start;
								result += (curr+24)->start ^ pivot->start;
								result += (curr+25)->start ^ pivot->start;
								result += (curr+26)->start ^ pivot->start;
								result += (curr+27)->start ^ pivot->start;
								pivot++;
							}
							break;
							
						case 29:
							while (pivot < last)
							{
								result += (curr+0)->start ^ pivot->start;
								result += (curr+1)->start ^ pivot->start;
								result += (curr+2)->start ^ pivot->start;
								result += (curr+3)->start ^ pivot->start;
								result += (curr+4)->start ^ pivot->start;
								result += (curr+5)->start ^ pivot->start;
								result += (curr+6)->start ^ pivot->start;
								result += (curr+7)->start ^ pivot->start;
								result += (curr+8)->start ^ pivot->start;
								result += (curr+9)->start ^ pivot->start;
								result += (curr+10)->start ^ pivot->start;
								result += (curr+11)->start ^ pivot->start;
								result += (curr+12)->start ^ pivot->start;
								result += (curr+13)->start ^ pivot->start;
								result += (curr+14)->start ^ pivot->start;
								result += (curr+15)->start ^ pivot->start;
								result += (curr+16)->start ^ pivot->start;
								result += (curr+17)->start ^ pivot->start;
								result += (curr+18)->start ^ pivot->start;
								result += (curr+19)->start ^ pivot->start;
								result += (curr+20)->start ^ pivot->start;
								result += (curr+21)->start ^ pivot->start;
								result += (curr+22)->start ^ pivot->start;
								result += (curr+23)->start ^ pivot->start;
								result += (curr+24)->start ^ pivot->start;
								result += (curr+25)->start ^ pivot->start;
								result += (curr+26)->start ^ pivot->start;
								result += (curr+27)->start ^ pivot->start;
								result += (curr+28)->start ^ pivot->start;
								pivot++;
							}
							break;
							
						case 30:
							while (pivot < last)
							{
								result += (curr+0)->start ^ pivot->start;
								result += (curr+1)->start ^ pivot->start;
								result += (curr+2)->start ^ pivot->start;
								result += (curr+3)->start ^ pivot->start;
								result += (curr+4)->start ^ pivot->start;
								result += (curr+5)->start ^ pivot->start;
								result += (curr+6)->start ^ pivot->start;
								result += (curr+7)->start ^ pivot->start;
								result += (curr+8)->start ^ pivot->start;
								result += (curr+9)->start ^ pivot->start;
								result += (curr+10)->start ^ pivot->start;
								result += (curr+11)->start ^ pivot->start;
								result += (curr+12)->start ^ pivot->start;
								result += (curr+13)->start ^ pivot->start;
								result += (curr+14)->start ^ pivot->start;
								result += (curr+15)->start ^ pivot->start;
								result += (curr+16)->start ^ pivot->start;
								result += (curr+17)->start ^ pivot->start;
								result += (curr+18)->start ^ pivot->start;
								result += (curr+19)->start ^ pivot->start;
								result += (curr+20)->start ^ pivot->start;
								result += (curr+21)->start ^ pivot->start;
								result += (curr+22)->start ^ pivot->start;
								result += (curr+23)->start ^ pivot->start;
								result += (curr+24)->start ^ pivot->start;
								result += (curr+25)->start ^ pivot->start;
								result += (curr+26)->start ^ pivot->start;
								result += (curr+27)->start ^ pivot->start;
								result += (curr+28)->start ^ pivot->start;
								result += (curr+29)->start ^ pivot->start;
								pivot++;
							}
							break;
							
						case 31:
							while (pivot < last)
							{
								result += (curr+0)->start ^ pivot->start;
								result += (curr+1)->start ^ pivot->start;
								result += (curr+2)->start ^ pivot->start;
								result += (curr+3)->start ^ pivot->start;
								result += (curr+4)->start ^ pivot->start;
								result += (curr+5)->start ^ pivot->start;
								result += (curr+6)->start ^ pivot->start;
								result += (curr+7)->start ^ pivot->start;
								result += (curr+8)->start ^ pivot->start;
								result += (curr+9)->start ^ pivot->start;
								result += (curr+10)->start ^ pivot->start;
								result += (curr+11)->start ^ pivot->start;
								result += (curr+12)->start ^ pivot->start;
								result += (curr+13)->start ^ pivot->start;
								result += (curr+14)->start ^ pivot->start;
								result += (curr+15)->start ^ pivot->start;
								result += (curr+16)->start ^ pivot->start;
								result += (curr+17)->start ^ pivot->start;
								result += (curr+18)->start ^ pivot->start;
								result += (curr+19)->start ^ pivot->start;
								result += (curr+20)->start ^ pivot->start;
								result += (curr+21)->start ^ pivot->start;
								result += (curr+22)->start ^ pivot->start;
								result += (curr+23)->start ^ pivot->start;
								result += (curr+24)->start ^ pivot->start;
								result += (curr+25)->start ^ pivot->start;
								result += (curr+26)->start ^ pivot->start;
								result += (curr+27)->start ^ pivot->start;
								result += (curr+28)->start ^ pivot->start;
								result += (curr+29)->start ^ pivot->start;
								result += (curr+30)->start ^ pivot->start;
								pivot++;
							}
							break;
							
						case 32:
							while (pivot < last)
							{
								result += (curr+0)->start ^ pivot->start;
								result += (curr+1)->start ^ pivot->start;
								result += (curr+2)->start ^ pivot->start;
								result += (curr+3)->start ^ pivot->start;
								result += (curr+4)->start ^ pivot->start;
								result += (curr+5)->start ^ pivot->start;
								result += (curr+6)->start ^ pivot->start;
								result += (curr+7)->start ^ pivot->start;
								result += (curr+8)->start ^ pivot->start;
								result += (curr+9)->start ^ pivot->start;
								result += (curr+10)->start ^ pivot->start;
								result += (curr+11)->start ^ pivot->start;
								result += (curr+12)->start ^ pivot->start;
								result += (curr+13)->start ^ pivot->start;
								result += (curr+14)->start ^ pivot->start;
								result += (curr+15)->start ^ pivot->start;
								result += (curr+16)->start ^ pivot->start;
								result += (curr+17)->start ^ pivot->start;
								result += (curr+18)->start ^ pivot->start;
								result += (curr+19)->start ^ pivot->start;
								result += (curr+20)->start ^ pivot->start;
								result += (curr+21)->start ^ pivot->start;
								result += (curr+22)->start ^ pivot->start;
								result += (curr+23)->start ^ pivot->start;
								result += (curr+24)->start ^ pivot->start;
								result += (curr+25)->start ^ pivot->start;
								result += (curr+26)->start ^ pivot->start;
								result += (curr+27)->start ^ pivot->start;
								result += (curr+28)->start ^ pivot->start;
								result += (curr+29)->start ^ pivot->start;
								result += (curr+30)->start ^ pivot->start;
								result += (curr+31)->start ^ pivot->start;
								pivot++;
							}
							break;
							
						default:
							while (last-pivot >= 32)
							{
								for (auto k = curr; k != lastGR; k++)
								{
									result += k->start ^ (pivot+0)->start;
									result += k->start ^ (pivot+1)->start;
									result += k->start ^ (pivot+2)->start;
									result += k->start ^ (pivot+3)->start;
									result += k->start ^ (pivot+4)->start;
									result += k->start ^ (pivot+5)->start;
									result += k->start ^ (pivot+6)->start;
									result += k->start ^ (pivot+7)->start;
									result += k->start ^ (pivot+8)->start;
									result += k->start ^ (pivot+9)->start;
									result += k->start ^ (pivot+10)->start;
									result += k->start ^ (pivot+11)->start;
									result += k->start ^ (pivot+12)->start;
									result += k->start ^ (pivot+13)->start;
									result += k->start ^ (pivot+14)->start;
									result += k->start ^ (pivot+15)->start;
									result += k->start ^ (pivot+16)->start;
									result += k->start ^ (pivot+17)->start;
									result += k->start ^ (pivot+18)->start;
									result += k->start ^ (pivot+19)->start;
									result += k->start ^ (pivot+20)->start;
									result += k->start ^ (pivot+21)->start;
									result += k->start ^ (pivot+22)->start;
									result += k->start ^ (pivot+23)->start;
									result += k->start ^ (pivot+24)->start;
									result += k->start ^ (pivot+25)->start;
									result += k->start ^ (pivot+26)->start;
									result += k->start ^ (pivot+27)->start;
									result += k->start ^ (pivot+28)->start;
									result += k->start ^ (pivot+29)->start;
									result += k->start ^ (pivot+30)->start;
									result += k->start ^ (pivot+31)->start;
								}
								pivot += 32;
							}
							
							if (last-pivot >= 16)
							{
								for (auto k = curr; k != lastGR; k++)
								{
									result += k->start ^ (pivot+0)->start;
									result += k->start ^ (pivot+1)->start;
									result += k->start ^ (pivot+2)->start;
									result += k->start ^ (pivot+3)->start;
									result += k->start ^ (pivot+4)->start;
									result += k->start ^ (pivot+5)->start;
									result += k->start ^ (pivot+6)->start;
									result += k->start ^ (pivot+7)->start;
									result += k->start ^ (pivot+8)->start;
									result += k->start ^ (pivot+9)->start;
									result += k->start ^ (pivot+10)->start;
									result += k->start ^ (pivot+11)->start;
									result += k->start ^ (pivot+12)->start;
									result += k->start ^ (pivot+13)->start;
									result += k->start ^ (pivot+14)->start;
									result += k->start ^ (pivot+15)->start;
								}
								pivot += 16;
							}
							
							if (last-pivot >= 8)
							{
								for (auto k = curr; k != lastGR; k++)
								{
									result += k->start ^ (pivot+0)->start;
									result += k->start ^ (pivot+1)->start;
									result += k->start ^ (pivot+2)->start;
									result += k->start ^ (pivot+3)->start;
									result += k->start ^ (pivot+4)->start;
									result += k->start ^ (pivot+5)->start;
									result += k->start ^ (pivot+6)->start;
									result += k->start ^ (pivot+7)->start;
								}
								pivot += 8;
							}
							
							if (last-pivot >= 4)
							{
								for (auto k = curr; k != lastGR; k++)
								{
									result += k->start ^ (pivot+0)->start;
									result += k->start ^ (pivot+1)->start;
									result += k->start ^ (pivot+2)->start;
									result += k->start ^ (pivot+3)->start;
								}
								pivot += 4;
							}
							
							while (pivot < last)
							{
								auto k = curr;
								while (lastGR-k >= 32)
								{
									result += (k+0)->start ^ pivot->start;
									result += (k+1)->start ^ pivot->start;
									result += (k+2)->start ^ pivot->start;
									result += (k+3)->start ^ pivot->start;
									result += (k+4)->start ^ pivot->start;
									result += (k+5)->start ^ pivot->start;
									result += (k+6)->start ^ pivot->start;
									result += (k+7)->start ^ pivot->start;
									result += (k+8)->start ^ pivot->start;
									result += (k+9)->start ^ pivot->start;
									result += (k+10)->start ^ pivot->start;
									result += (k+11)->start ^ pivot->start;
									result += (k+12)->start ^ pivot->start;
									result += (k+13)->start ^ pivot->start;
									result += (k+14)->start ^ pivot->start;
									result += (k+15)->start ^ pivot->start;
									result += (k+16)->start ^ pivot->start;
									result += (k+17)->start ^ pivot->start;
									result += (k+18)->start ^ pivot->start;
									result += (k+19)->start ^ pivot->start;
									result += (k+20)->start ^ pivot->start;
									result += (k+21)->start ^ pivot->start;
									result += (k+22)->start ^ pivot->start;
									result += (k+23)->start ^ pivot->start;
									result += (k+24)->start ^ pivot->start;
									result += (k+25)->start ^ pivot->start;
									result += (k+26)->start ^ pivot->start;
									result += (k+27)->start ^ pivot->start;
									result += (k+28)->start ^ pivot->start;
									result += (k+29)->start ^ pivot->start;
									result += (k+30)->start ^ pivot->start;
									result += (k+31)->start ^ pivot->start;
									k += 32;
								}
								
								if (lastGR-k >= 16)
								{
									result += (k+0)->start ^ pivot->start;
									result += (k+1)->start ^ pivot->start;
									result += (k+2)->start ^ pivot->start;
									result += (k+3)->start ^ pivot->start;
									result += (k+4)->start ^ pivot->start;
									result += (k+5)->start ^ pivot->start;
									result += (k+6)->start ^ pivot->start;
									result += (k+7)->start ^ pivot->start;
									result += (k+8)->start ^ pivot->start;
									result += (k+9)->start ^ pivot->start;
									result += (k+10)->start ^ pivot->start;
									result += (k+11)->start ^ pivot->start;
									result += (k+12)->start ^ pivot->start;
									result += (k+13)->start ^ pivot->start;
									result += (k+14)->start ^ pivot->start;
									result += (k+15)->start ^ pivot->start;
									k += 16;
								}
								
								if (lastGR-k >= 8)
								{
									result += (k+0)->start ^ pivot->start;
									result += (k+1)->start ^ pivot->start;
									result += (k+2)->start ^ pivot->start;
									result += (k+3)->start ^ pivot->start;
									result += (k+4)->start ^ pivot->start;
									result += (k+5)->start ^ pivot->start;
									result += (k+6)->start ^ pivot->start;
									result += (k+7)->start ^ pivot->start;
									k += 8;
								}
								
								if (lastGR-k >= 4)
								{
									result += (k+0)->start ^ pivot->start;
									result += (k+1)->start ^ pivot->start;
									result += (k+2)->start ^ pivot->start;
									result += (k+3)->start ^ pivot->start;
									k += 4;
								}
								
								while (k != lastGR)
								{
									result += k->start ^ pivot->start; k++;
								}
								
								pivot++;
							}
							break;
					}
				}
				
				// Sweep the last bucket.
				auto last = BIS[cbucket_id].last;
				switch (bufferSize)
				{
					case 1:
						while ((last-pivot >= 32) && (curr->end >= (pivot+31)->start))
						{
							result += curr->start ^ (pivot+0)->start;
							result += curr->start ^ (pivot+1)->start;
							result += curr->start ^ (pivot+2)->start;
							result += curr->start ^ (pivot+3)->start;
							result += curr->start ^ (pivot+4)->start;
							result += curr->start ^ (pivot+5)->start;
							result += curr->start ^ (pivot+6)->start;
							result += curr->start ^ (pivot+7)->start;
							result += curr->start ^ (pivot+8)->start;
							result += curr->start ^ (pivot+9)->start;
							result += curr->start ^ (pivot+10)->start;
							result += curr->start ^ (pivot+11)->start;
							result += curr->start ^ (pivot+12)->start;
							result += curr->start ^ (pivot+13)->start;
							result += curr->start ^ (pivot+14)->start;
							result += curr->start ^ (pivot+15)->start;
							result += curr->start ^ (pivot+16)->start;
							result += curr->start ^ (pivot+17)->start;
							result += curr->start ^ (pivot+18)->start;
							result += curr->start ^ (pivot+19)->start;
							result += curr->start ^ (pivot+20)->start;
							result += curr->start ^ (pivot+21)->start;
							result += curr->start ^ (pivot+22)->start;
							result += curr->start ^ (pivot+23)->start;
							result += curr->start ^ (pivot+24)->start;
							result += curr->start ^ (pivot+25)->start;
							result += curr->start ^ (pivot+26)->start;
							result += curr->start ^ (pivot+27)->start;
							result += curr->start ^ (pivot+28)->start;
							result += curr->start ^ (pivot+29)->start;
							result += curr->start ^ (pivot+30)->start;
							result += curr->start ^ (pivot+31)->start;
							pivot += 32;
						}
						
						while ((pivot < last) && (pivot->start <= curr->end))
						{
							result += curr->start ^ pivot->start;
							pivot++;
						}
						break;
						
					case 2:
						while ((last-pivot >= 16) && (curr->end >= (pivot+15)->start))
						{
							result += (curr+0)->start ^ (pivot+0)->start;
							result += (curr+1)->start ^ (pivot+0)->start;
							result += (curr+0)->start ^ (pivot+1)->start;
							result += (curr+1)->start ^ (pivot+1)->start;
							result += (curr+0)->start ^ (pivot+2)->start;
							result += (curr+1)->start ^ (pivot+2)->start;
							result += (curr+0)->start ^ (pivot+3)->start;
							result += (curr+1)->start ^ (pivot+3)->start;
							result += (curr+0)->start ^ (pivot+4)->start;
							result += (curr+1)->start ^ (pivot+4)->start;
							result += (curr+0)->start ^ (pivot+5)->start;
							result += (curr+1)->start ^ (pivot+5)->start;
							result += (curr+0)->start ^ (pivot+6)->start;
							result += (curr+1)->start ^ (pivot+6)->start;
							result += (curr+0)->start ^ (pivot+7)->start;
							result += (curr+1)->start ^ (pivot+7)->start;
							result += (curr+0)->start ^ (pivot+8)->start;
							result += (curr+1)->start ^ (pivot+8)->start;
							result += (curr+0)->start ^ (pivot+9)->start;
							result += (curr+1)->start ^ (pivot+9)->start;
							result += (curr+0)->start ^ (pivot+10)->start;
							result += (curr+1)->start ^ (pivot+10)->start;
							result += (curr+0)->start ^ (pivot+11)->start;
							result += (curr+1)->start ^ (pivot+11)->start;
							result += (curr+0)->start ^ (pivot+12)->start;
							result += (curr+1)->start ^ (pivot+12)->start;
							result += (curr+0)->start ^ (pivot+13)->start;
							result += (curr+1)->start ^ (pivot+13)->start;
							result += (curr+0)->start ^ (pivot+14)->start;
							result += (curr+1)->start ^ (pivot+14)->start;
							result += (curr+0)->start ^ (pivot+15)->start;
							result += (curr+1)->start ^ (pivot+15)->start;
							pivot += 16;
						}
						
						while ((pivot < last) && (pivot->start <= curr->end))
						{
							result += (curr+0)->start ^ pivot->start;
							result += (curr+1)->start ^ pivot->start;
							pivot++;
						}
						break;
						
					case 3:
						while ((last-pivot >= 8) && (curr->end >= (pivot+7)->start))
						{
							result += (curr+0)->start ^ (pivot+0)->start;
							result += (curr+1)->start ^ (pivot+0)->start;
							result += (curr+2)->start ^ (pivot+0)->start;
							result += (curr+0)->start ^ (pivot+1)->start;
							result += (curr+1)->start ^ (pivot+1)->start;
							result += (curr+2)->start ^ (pivot+1)->start;
							result += (curr+0)->start ^ (pivot+2)->start;
							result += (curr+1)->start ^ (pivot+2)->start;
							result += (curr+2)->start ^ (pivot+2)->start;
							result += (curr+0)->start ^ (pivot+3)->start;
							result += (curr+1)->start ^ (pivot+3)->start;
							result += (curr+2)->start ^ (pivot+3)->start;
							result += (curr+0)->start ^ (pivot+4)->start;
							result += (curr+1)->start ^ (pivot+4)->start;
							result += (curr+2)->start ^ (pivot+4)->start;
							result += (curr+0)->start ^ (pivot+5)->start;
							result += (curr+1)->start ^ (pivot+5)->start;
							result += (curr+2)->start ^ (pivot+5)->start;
							result += (curr+0)->start ^ (pivot+6)->start;
							result += (curr+1)->start ^ (pivot+6)->start;
							result += (curr+2)->start ^ (pivot+6)->start;
							result += (curr+0)->start ^ (pivot+7)->start;
							result += (curr+1)->start ^ (pivot+7)->start;
							result += (curr+2)->start ^ (pivot+7)->start;
							pivot += 8;
						}
						
						while ((pivot < last) && (pivot->start <= curr->end))
						{
							result += (curr+0)->start ^ pivot->start;
							result += (curr+1)->start ^ pivot->start;
							result += (curr+2)->start ^ pivot->start;
							pivot++;
						}
						break;
						
					case 4:
						while ((last-pivot >= 8) && (curr->end >= (pivot+7)->start))
						{
							result += (curr+0)->start ^ (pivot+0)->start;
							result += (curr+1)->start ^ (pivot+0)->start;
							result += (curr+2)->start ^ (pivot+0)->start;
							result += (curr+3)->start ^ (pivot+0)->start;
							result += (curr+0)->start ^ (pivot+1)->start;
							result += (curr+1)->start ^ (pivot+1)->start;
							result += (curr+2)->start ^ (pivot+1)->start;
							result += (curr+3)->start ^ (pivot+1)->start;
							result += (curr+0)->start ^ (pivot+2)->start;
							result += (curr+1)->start ^ (pivot+2)->start;
							result += (curr+2)->start ^ (pivot+2)->start;
							result += (curr+3)->start ^ (pivot+2)->start;
							result += (curr+0)->start ^ (pivot+3)->start;
							result += (curr+1)->start ^ (pivot+3)->start;
							result += (curr+2)->start ^ (pivot+3)->start;
							result += (curr+3)->start ^ (pivot+3)->start;
							result += (curr+0)->start ^ (pivot+4)->start;
							result += (curr+1)->start ^ (pivot+4)->start;
							result += (curr+2)->start ^ (pivot+4)->start;
							result += (curr+3)->start ^ (pivot+4)->start;
							result += (curr+0)->start ^ (pivot+5)->start;
							result += (curr+1)->start ^ (pivot+5)->start;
							result += (curr+2)->start ^ (pivot+5)->start;
							result += (curr+3)->start ^ (pivot+5)->start;
							result += (curr+0)->start ^ (pivot+6)->start;
							result += (curr+1)->start ^ (pivot+6)->start;
							result += (curr+2)->start ^ (pivot+6)->start;
							result += (curr+3)->start ^ (pivot+6)->start;
							result += (curr+0)->start ^ (pivot+7)->start;
							result += (curr+1)->start ^ (pivot+7)->start;
							result += (curr+2)->start ^ (pivot+7)->start;
							result += (curr+3)->start ^ (pivot+7)->start;
							pivot += 8;
						}
						
						while ((pivot < last) && (pivot->start <= curr->end))
						{
							result += (curr+0)->start ^ pivot->start;
							result += (curr+1)->start ^ pivot->start;
							result += (curr+2)->start ^ pivot->start;
							result += (curr+3)->start ^ pivot->start;
							pivot++;
						}
						break;
						
					case 5:
						while ((pivot < last) && (pivot->start <= curr->end))
						{
							result += (curr+0)->start ^ pivot->start;
							result += (curr+1)->start ^ pivot->start;
							result += (curr+2)->start ^ pivot->start;
							result += (curr+3)->start ^ pivot->start;
							result += (curr+4)->start ^ pivot->start;
							pivot++;
						}
						break;
						
					case 6:
						while ((pivot < last) && (pivot->start <= curr->end))
						{
							result += (curr+0)->start ^ pivot->start;
							result += (curr+1)->start ^ pivot->start;
							result += (curr+2)->start ^ pivot->start;
							result += (curr+3)->start ^ pivot->start;
							result += (curr+4)->start ^ pivot->start;
							result += (curr+5)->start ^ pivot->start;
							pivot++;
						}
						break;
						
					case 7:
						while ((pivot < last) && (pivot->start <= curr->end))
						{
							result += (curr+0)->start ^ pivot->start;
							result += (curr+1)->start ^ pivot->start;
							result += (curr+2)->start ^ pivot->start;
							result += (curr+3)->start ^ pivot->start;
							result += (curr+4)->start ^ pivot->start;
							result += (curr+5)->start ^ pivot->start;
							result += (curr+6)->start ^ pivot->start;
							pivot++;
						}
						break;
						
					case 8:
						while ((pivot < last) && (pivot->start <= curr->end))
						{
							result += (curr+0)->start ^ pivot->start;
							result += (curr+1)->start ^ pivot->start;
							result += (curr+2)->start ^ pivot->start;
							result += (curr+3)->start ^ pivot->start;
							result += (curr+4)->start ^ pivot->start;
							result += (curr+5)->start ^ pivot->start;
							result += (curr+6)->start ^ pivot->start;
							result += (curr+7)->start ^ pivot->start;
							pivot++;
						}
						break;
						
					case 9:
						while ((pivot < last) && (pivot->start <= curr->end))
						{
							result += (curr+0)->start ^ pivot->start;
							result += (curr+1)->start ^ pivot->start;
							result += (curr+2)->start ^ pivot->start;
							result += (curr+3)->start ^ pivot->start;
							result += (curr+4)->start ^ pivot->start;
							result += (curr+5)->start ^ pivot->start;
							result += (curr+6)->start ^ pivot->start;
							result += (curr+7)->start ^ pivot->start;
							result += (curr+8)->start ^ pivot->start;
							pivot++;
						}
						break;
						
					case 10:
						while ((pivot < last) && (pivot->start <= curr->end))
						{
							result += (curr+0)->start ^ pivot->start;
							result += (curr+1)->start ^ pivot->start;
							result += (curr+2)->start ^ pivot->start;
							result += (curr+3)->start ^ pivot->start;
							result += (curr+4)->start ^ pivot->start;
							result += (curr+5)->start ^ pivot->start;
							result += (curr+6)->start ^ pivot->start;
							result += (curr+7)->start ^ pivot->start;
							result += (curr+8)->start ^ pivot->start;
							result += (curr+9)->start ^ pivot->start;
							pivot++;
						}
						break;
						
					case 11:
						while ((pivot < last) && (pivot->start <= curr->end))
						{
							result += (curr+0)->start ^ pivot->start;
							result += (curr+1)->start ^ pivot->start;
							result += (curr+2)->start ^ pivot->start;
							result += (curr+3)->start ^ pivot->start;
							result += (curr+4)->start ^ pivot->start;
							result += (curr+5)->start ^ pivot->start;
							result += (curr+6)->start ^ pivot->start;
							result += (curr+7)->start ^ pivot->start;
							result += (curr+8)->start ^ pivot->start;
							result += (curr+9)->start ^ pivot->start;
							result += (curr+10)->start ^ pivot->start;
							pivot++;
						}
						break;
						
					case 12:
						while ((pivot < last) && (pivot->start <= curr->end))
						{
							result += (curr+0)->start ^ pivot->start;
							result += (curr+1)->start ^ pivot->start;
							result += (curr+2)->start ^ pivot->start;
							result += (curr+3)->start ^ pivot->start;
							result += (curr+4)->start ^ pivot->start;
							result += (curr+5)->start ^ pivot->start;
							result += (curr+6)->start ^ pivot->start;
							result += (curr+7)->start ^ pivot->start;
							result += (curr+8)->start ^ pivot->start;
							result += (curr+9)->start ^ pivot->start;
							result += (curr+10)->start ^ pivot->start;
							result += (curr+11)->start ^ pivot->start;
							pivot++;
						}
						break;
						
					case 13:
						while ((pivot < last) && (pivot->start <= curr->end))
						{
							result += (curr+0)->start ^ pivot->start;
							result += (curr+1)->start ^ pivot->start;
							result += (curr+2)->start ^ pivot->start;
							result += (curr+3)->start ^ pivot->start;
							result += (curr+4)->start ^ pivot->start;
							result += (curr+5)->start ^ pivot->start;
							result += (curr+6)->start ^ pivot->start;
							result += (curr+7)->start ^ pivot->start;
							result += (curr+8)->start ^ pivot->start;
							result += (curr+9)->start ^ pivot->start;
							result += (curr+10)->start ^ pivot->start;
							result += (curr+11)->start ^ pivot->start;
							result += (curr+12)->start ^ pivot->start;
							pivot++;
						}
						break;
						
					case 14:
						while ((pivot < last) && (pivot->start <= curr->end))
						{
							result += (curr+0)->start ^ pivot->start;
							result += (curr+1)->start ^ pivot->start;
							result += (curr+2)->start ^ pivot->start;
							result += (curr+3)->start ^ pivot->start;
							result += (curr+4)->start ^ pivot->start;
							result += (curr+5)->start ^ pivot->start;
							result += (curr+6)->start ^ pivot->start;
							result += (curr+7)->start ^ pivot->start;
							result += (curr+8)->start ^ pivot->start;
							result += (curr+9)->start ^ pivot->start;
							result += (curr+10)->start ^ pivot->start;
							result += (curr+11)->start ^ pivot->start;
							result += (curr+12)->start ^ pivot->start;
							result += (curr+13)->start ^ pivot->start;
							pivot++;
						}
						break;
						
					case 15:
						while ((pivot < last) && (pivot->start <= curr->end))
						{
							result += (curr+0)->start ^ pivot->start;
							result += (curr+1)->start ^ pivot->start;
							result += (curr+2)->start ^ pivot->start;
							result += (curr+3)->start ^ pivot->start;
							result += (curr+4)->start ^ pivot->start;
							result += (curr+5)->start ^ pivot->start;
							result += (curr+6)->start ^ pivot->start;
							result += (curr+7)->start ^ pivot->start;
							result += (curr+8)->start ^ pivot->start;
							result += (curr+9)->start ^ pivot->start;
							result += (curr+10)->start ^ pivot->start;
							result += (curr+11)->start ^ pivot->start;
							result += (curr+12)->start ^ pivot->start;
							result += (curr+13)->start ^ pivot->start;
							result += (curr+14)->start ^ pivot->start;
							pivot++;
						}
						break;
						
					case 16:
						while ((pivot < last) && (pivot->start <= curr->end))
						{
							result += (curr+0)->start ^ pivot->start;
							result += (curr+1)->start ^ pivot->start;
							result += (curr+2)->start ^ pivot->start;
							result += (curr+3)->start ^ pivot->start;
							result += (curr+4)->start ^ pivot->start;
							result += (curr+5)->start ^ pivot->start;
							result += (curr+6)->start ^ pivot->start;
							result += (curr+7)->start ^ pivot->start;
							result += (curr+8)->start ^ pivot->start;
							result += (curr+9)->start ^ pivot->start;
							result += (curr+10)->start ^ pivot->start;
							result += (curr+11)->start ^ pivot->start;
							result += (curr+12)->start ^ pivot->start;
							result += (curr+13)->start ^ pivot->start;
							result += (curr+14)->start ^ pivot->start;
							result += (curr+15)->start ^ pivot->start;
							pivot++;
						}
						break;
						
					case 17:
						while ((pivot < last) && (pivot->start <= curr->end))
						{
							result += (curr+0)->start ^ pivot->start;
							result += (curr+1)->start ^ pivot->start;
							result += (curr+2)->start ^ pivot->start;
							result += (curr+3)->start ^ pivot->start;
							result += (curr+4)->start ^ pivot->start;
							result += (curr+5)->start ^ pivot->start;
							result += (curr+6)->start ^ pivot->start;
							result += (curr+7)->start ^ pivot->start;
							result += (curr+8)->start ^ pivot->start;
							result += (curr+9)->start ^ pivot->start;
							result += (curr+10)->start ^ pivot->start;
							result += (curr+11)->start ^ pivot->start;
							result += (curr+12)->start ^ pivot->start;
							result += (curr+13)->start ^ pivot->start;
							result += (curr+14)->start ^ pivot->start;
							result += (curr+15)->start ^ pivot->start;
							result += (curr+16)->start ^ pivot->start;
							pivot++;
						}
						break;
						
					case 18:
						while ((pivot < last) && (pivot->start <= curr->end))
						{
							result += (curr+0)->start ^ pivot->start;
							result += (curr+1)->start ^ pivot->start;
							result += (curr+2)->start ^ pivot->start;
							result += (curr+3)->start ^ pivot->start;
							result += (curr+4)->start ^ pivot->start;
							result += (curr+5)->start ^ pivot->start;
							result += (curr+6)->start ^ pivot->start;
							result += (curr+7)->start ^ pivot->start;
							result += (curr+8)->start ^ pivot->start;
							result += (curr+9)->start ^ pivot->start;
							result += (curr+10)->start ^ pivot->start;
							result += (curr+11)->start ^ pivot->start;
							result += (curr+12)->start ^ pivot->start;
							result += (curr+13)->start ^ pivot->start;
							result += (curr+14)->start ^ pivot->start;
							result += (curr+15)->start ^ pivot->start;
							result += (curr+16)->start ^ pivot->start;
							result += (curr+17)->start ^ pivot->start;
							pivot++;
						}
						break;
						
					case 19:
						while ((pivot < last) && (pivot->start <= curr->end))
						{
							result += (curr+0)->start ^ pivot->start;
							result += (curr+1)->start ^ pivot->start;
							result += (curr+2)->start ^ pivot->start;
							result += (curr+3)->start ^ pivot->start;
							result += (curr+4)->start ^ pivot->start;
							result += (curr+5)->start ^ pivot->start;
							result += (curr+6)->start ^ pivot->start;
							result += (curr+7)->start ^ pivot->start;
							result += (curr+8)->start ^ pivot->start;
							result += (curr+9)->start ^ pivot->start;
							result += (curr+10)->start ^ pivot->start;
							result += (curr+11)->start ^ pivot->start;
							result += (curr+12)->start ^ pivot->start;
							result += (curr+13)->start ^ pivot->start;
							result += (curr+14)->start ^ pivot->start;
							result += (curr+15)->start ^ pivot->start;
							result += (curr+16)->start ^ pivot->start;
							result += (curr+17)->start ^ pivot->start;
							result += (curr+18)->start ^ pivot->start;
							pivot++;
						}
						break;
						
					case 20:
						while ((pivot < last) && (pivot->start <= curr->end))
						{
							result += (curr+0)->start ^ pivot->start;
							result += (curr+1)->start ^ pivot->start;
							result += (curr+2)->start ^ pivot->start;
							result += (curr+3)->start ^ pivot->start;
							result += (curr+4)->start ^ pivot->start;
							result += (curr+5)->start ^ pivot->start;
							result += (curr+6)->start ^ pivot->start;
							result += (curr+7)->start ^ pivot->start;
							result += (curr+8)->start ^ pivot->start;
							result += (curr+9)->start ^ pivot->start;
							result += (curr+10)->start ^ pivot->start;
							result += (curr+11)->start ^ pivot->start;
							result += (curr+12)->start ^ pivot->start;
							result += (curr+13)->start ^ pivot->start;
							result += (curr+14)->start ^ pivot->start;
							result += (curr+15)->start ^ pivot->start;
							result += (curr+16)->start ^ pivot->start;
							result += (curr+17)->start ^ pivot->start;
							result += (curr+18)->start ^ pivot->start;
							result += (curr+19)->start ^ pivot->start;
							pivot++;
						}
						break;
						
					case 21:
						while ((pivot < last) && (pivot->start <= curr->end))
						{
							result += (curr+0)->start ^ pivot->start;
							result += (curr+1)->start ^ pivot->start;
							result += (curr+2)->start ^ pivot->start;
							result += (curr+3)->start ^ pivot->start;
							result += (curr+4)->start ^ pivot->start;
							result += (curr+5)->start ^ pivot->start;
							result += (curr+6)->start ^ pivot->start;
							result += (curr+7)->start ^ pivot->start;
							result += (curr+8)->start ^ pivot->start;
							result += (curr+9)->start ^ pivot->start;
							result += (curr+10)->start ^ pivot->start;
							result += (curr+11)->start ^ pivot->start;
							result += (curr+12)->start ^ pivot->start;
							result += (curr+13)->start ^ pivot->start;
							result += (curr+14)->start ^ pivot->start;
							result += (curr+15)->start ^ pivot->start;
							result += (curr+16)->start ^ pivot->start;
							result += (curr+17)->start ^ pivot->start;
							result += (curr+18)->start ^ pivot->start;
							result += (curr+19)->start ^ pivot->start;
							result += (curr+20)->start ^ pivot->start;
							pivot++;
						}
						break;
						
					case 22:
						while ((pivot < last) && (pivot->start <= curr->end))
						{
							result += (curr+0)->start ^ pivot->start;
							result += (curr+1)->start ^ pivot->start;
							result += (curr+2)->start ^ pivot->start;
							result += (curr+3)->start ^ pivot->start;
							result += (curr+4)->start ^ pivot->start;
							result += (curr+5)->start ^ pivot->start;
							result += (curr+6)->start ^ pivot->start;
							result += (curr+7)->start ^ pivot->start;
							result += (curr+8)->start ^ pivot->start;
							result += (curr+9)->start ^ pivot->start;
							result += (curr+10)->start ^ pivot->start;
							result += (curr+11)->start ^ pivot->start;
							result += (curr+12)->start ^ pivot->start;
							result += (curr+13)->start ^ pivot->start;
							result += (curr+14)->start ^ pivot->start;
							result += (curr+15)->start ^ pivot->start;
							result += (curr+16)->start ^ pivot->start;
							result += (curr+17)->start ^ pivot->start;
							result += (curr+18)->start ^ pivot->start;
							result += (curr+19)->start ^ pivot->start;
							result += (curr+20)->start ^ pivot->start;
							result += (curr+21)->start ^ pivot->start;
							pivot++;
						}
						break;
						
					case 23:
						while ((pivot < last) && (pivot->start <= curr->end))
						{
							result += (curr+0)->start ^ pivot->start;
							result += (curr+1)->start ^ pivot->start;
							result += (curr+2)->start ^ pivot->start;
							result += (curr+3)->start ^ pivot->start;
							result += (curr+4)->start ^ pivot->start;
							result += (curr+5)->start ^ pivot->start;
							result += (curr+6)->start ^ pivot->start;
							result += (curr+7)->start ^ pivot->start;
							result += (curr+8)->start ^ pivot->start;
							result += (curr+9)->start ^ pivot->start;
							result += (curr+10)->start ^ pivot->start;
							result += (curr+11)->start ^ pivot->start;
							result += (curr+12)->start ^ pivot->start;
							result += (curr+13)->start ^ pivot->start;
							result += (curr+14)->start ^ pivot->start;
							result += (curr+15)->start ^ pivot->start;
							result += (curr+16)->start ^ pivot->start;
							result += (curr+17)->start ^ pivot->start;
							result += (curr+18)->start ^ pivot->start;
							result += (curr+19)->start ^ pivot->start;
							result += (curr+20)->start ^ pivot->start;
							result += (curr+21)->start ^ pivot->start;
							result += (curr+22)->start ^ pivot->start;
							pivot++;
						}
						break;
						
					case 24:
						while ((pivot < last) && (pivot->start <= curr->end))
						{
							result += (curr+0)->start ^ pivot->start;
							result += (curr+1)->start ^ pivot->start;
							result += (curr+2)->start ^ pivot->start;
							result += (curr+3)->start ^ pivot->start;
							result += (curr+4)->start ^ pivot->start;
							result += (curr+5)->start ^ pivot->start;
							result += (curr+6)->start ^ pivot->start;
							result += (curr+7)->start ^ pivot->start;
							result += (curr+8)->start ^ pivot->start;
							result += (curr+9)->start ^ pivot->start;
							result += (curr+10)->start ^ pivot->start;
							result += (curr+11)->start ^ pivot->start;
							result += (curr+12)->start ^ pivot->start;
							result += (curr+13)->start ^ pivot->start;
							result += (curr+14)->start ^ pivot->start;
							result += (curr+15)->start ^ pivot->start;
							result += (curr+16)->start ^ pivot->start;
							result += (curr+17)->start ^ pivot->start;
							result += (curr+18)->start ^ pivot->start;
							result += (curr+19)->start ^ pivot->start;
							result += (curr+20)->start ^ pivot->start;
							result += (curr+21)->start ^ pivot->start;
							result += (curr+22)->start ^ pivot->start;
							result += (curr+23)->start ^ pivot->start;
							pivot++;
						}
						break;
						
					case 25:
						while ((pivot < last) && (pivot->start <= curr->end))
						{
							result += (curr+0)->start ^ pivot->start;
							result += (curr+1)->start ^ pivot->start;
							result += (curr+2)->start ^ pivot->start;
							result += (curr+3)->start ^ pivot->start;
							result += (curr+4)->start ^ pivot->start;
							result += (curr+5)->start ^ pivot->start;
							result += (curr+6)->start ^ pivot->start;
							result += (curr+7)->start ^ pivot->start;
							result += (curr+8)->start ^ pivot->start;
							result += (curr+9)->start ^ pivot->start;
							result += (curr+10)->start ^ pivot->start;
							result += (curr+11)->start ^ pivot->start;
							result += (curr+12)->start ^ pivot->start;
							result += (curr+13)->start ^ pivot->start;
							result += (curr+14)->start ^ pivot->start;
							result += (curr+15)->start ^ pivot->start;
							result += (curr+16)->start ^ pivot->start;
							result += (curr+17)->start ^ pivot->start;
							result += (curr+18)->start ^ pivot->start;
							result += (curr+19)->start ^ pivot->start;
							result += (curr+20)->start ^ pivot->start;
							result += (curr+21)->start ^ pivot->start;
							result += (curr+22)->start ^ pivot->start;
							result += (curr+23)->start ^ pivot->start;
							result += (curr+24)->start ^ pivot->start;
							pivot++;
						}
						break;
						
					case 26:
						while ((pivot < last) && (pivot->start <= curr->end))
						{
							result += (curr+0)->start ^ pivot->start;
							result += (curr+1)->start ^ pivot->start;
							result += (curr+2)->start ^ pivot->start;
							result += (curr+3)->start ^ pivot->start;
							result += (curr+4)->start ^ pivot->start;
							result += (curr+5)->start ^ pivot->start;
							result += (curr+6)->start ^ pivot->start;
							result += (curr+7)->start ^ pivot->start;
							result += (curr+8)->start ^ pivot->start;
							result += (curr+9)->start ^ pivot->start;
							result += (curr+10)->start ^ pivot->start;
							result += (curr+11)->start ^ pivot->start;
							result += (curr+12)->start ^ pivot->start;
							result += (curr+13)->start ^ pivot->start;
							result += (curr+14)->start ^ pivot->start;
							result += (curr+15)->start ^ pivot->start;
							result += (curr+16)->start ^ pivot->start;
							result += (curr+17)->start ^ pivot->start;
							result += (curr+18)->start ^ pivot->start;
							result += (curr+19)->start ^ pivot->start;
							result += (curr+20)->start ^ pivot->start;
							result += (curr+21)->start ^ pivot->start;
							result += (curr+22)->start ^ pivot->start;
							result += (curr+23)->start ^ pivot->start;
							result += (curr+24)->start ^ pivot->start;
							result += (curr+25)->start ^ pivot->start;
							pivot++;
						}
						break;
						
					case 27:
						while ((pivot < last) && (pivot->start <= curr->end))
						{
							result += (curr+0)->start ^ pivot->start;
							result += (curr+1)->start ^ pivot->start;
							result += (curr+2)->start ^ pivot->start;
							result += (curr+3)->start ^ pivot->start;
							result += (curr+4)->start ^ pivot->start;
							result += (curr+5)->start ^ pivot->start;
							result += (curr+6)->start ^ pivot->start;
							result += (curr+7)->start ^ pivot->start;
							result += (curr+8)->start ^ pivot->start;
							result += (curr+9)->start ^ pivot->start;
							result += (curr+10)->start ^ pivot->start;
							result += (curr+11)->start ^ pivot->start;
							result += (curr+12)->start ^ pivot->start;
							result += (curr+13)->start ^ pivot->start;
							result += (curr+14)->start ^ pivot->start;
							result += (curr+15)->start ^ pivot->start;
							result += (curr+16)->start ^ pivot->start;
							result += (curr+17)->start ^ pivot->start;
							result += (curr+18)->start ^ pivot->start;
							result += (curr+19)->start ^ pivot->start;
							result += (curr+20)->start ^ pivot->start;
							result += (curr+21)->start ^ pivot->start;
							result += (curr+22)->start ^ pivot->start;
							result += (curr+23)->start ^ pivot->start;
							result += (curr+24)->start ^ pivot->start;
							result += (curr+25)->start ^ pivot->start;
							result += (curr+26)->start ^ pivot->start;
							pivot++;
						}
						break;
						
					case 28:
						while ((pivot < last) && (pivot->start <= curr->end))
						{
							result += (curr+0)->start ^ pivot->start;
							result += (curr+1)->start ^ pivot->start;
							result += (curr+2)->start ^ pivot->start;
							result += (curr+3)->start ^ pivot->start;
							result += (curr+4)->start ^ pivot->start;
							result += (curr+5)->start ^ pivot->start;
							result += (curr+6)->start ^ pivot->start;
							result += (curr+7)->start ^ pivot->start;
							result += (curr+8)->start ^ pivot->start;
							result += (curr+9)->start ^ pivot->start;
							result += (curr+10)->start ^ pivot->start;
							result += (curr+11)->start ^ pivot->start;
							result += (curr+12)->start ^ pivot->start;
							result += (curr+13)->start ^ pivot->start;
							result += (curr+14)->start ^ pivot->start;
							result += (curr+15)->start ^ pivot->start;
							result += (curr+16)->start ^ pivot->start;
							result += (curr+17)->start ^ pivot->start;
							result += (curr+18)->start ^ pivot->start;
							result += (curr+19)->start ^ pivot->start;
							result += (curr+20)->start ^ pivot->start;
							result += (curr+21)->start ^ pivot->start;
							result += (curr+22)->start ^ pivot->start;
							result += (curr+23)->start ^ pivot->start;
							result += (curr+24)->start ^ pivot->start;
							result += (curr+25)->start ^ pivot->start;
							result += (curr+26)->start ^ pivot->start;
							result += (curr+27)->start ^ pivot->start;
							pivot++;
						}
						break;
						
					case 29:
						while ((pivot < last) && (pivot->start <= curr->end))
						{
							result += (curr+0)->start ^ pivot->start;
							result += (curr+1)->start ^ pivot->start;
							result += (curr+2)->start ^ pivot->start;
							result += (curr+3)->start ^ pivot->start;
							result += (curr+4)->start ^ pivot->start;
							result += (curr+5)->start ^ pivot->start;
							result += (curr+6)->start ^ pivot->start;
							result += (curr+7)->start ^ pivot->start;
							result += (curr+8)->start ^ pivot->start;
							result += (curr+9)->start ^ pivot->start;
							result += (curr+10)->start ^ pivot->start;
							result += (curr+11)->start ^ pivot->start;
							result += (curr+12)->start ^ pivot->start;
							result += (curr+13)->start ^ pivot->start;
							result += (curr+14)->start ^ pivot->start;
							result += (curr+15)->start ^ pivot->start;
							result += (curr+16)->start ^ pivot->start;
							result += (curr+17)->start ^ pivot->start;
							result += (curr+18)->start ^ pivot->start;
							result += (curr+19)->start ^ pivot->start;
							result += (curr+20)->start ^ pivot->start;
							result += (curr+21)->start ^ pivot->start;
							result += (curr+22)->start ^ pivot->start;
							result += (curr+23)->start ^ pivot->start;
							result += (curr+24)->start ^ pivot->start;
							result += (curr+25)->start ^ pivot->start;
							result += (curr+26)->start ^ pivot->start;
							result += (curr+27)->start ^ pivot->start;
							result += (curr+28)->start ^ pivot->start;
							pivot++;
						}
						break;
						
					case 30:
						while ((pivot < last) && (pivot->start <= curr->end))
						{
							result += (curr+0)->start ^ pivot->start;
							result += (curr+1)->start ^ pivot->start;
							result += (curr+2)->start ^ pivot->start;
							result += (curr+3)->start ^ pivot->start;
							result += (curr+4)->start ^ pivot->start;
							result += (curr+5)->start ^ pivot->start;
							result += (curr+6)->start ^ pivot->start;
							result += (curr+7)->start ^ pivot->start;
							result += (curr+8)->start ^ pivot->start;
							result += (curr+9)->start ^ pivot->start;
							result += (curr+10)->start ^ pivot->start;
							result += (curr+11)->start ^ pivot->start;
							result += (curr+12)->start ^ pivot->start;
							result += (curr+13)->start ^ pivot->start;
							result += (curr+14)->start ^ pivot->start;
							result += (curr+15)->start ^ pivot->start;
							result += (curr+16)->start ^ pivot->start;
							result += (curr+17)->start ^ pivot->start;
							result += (curr+18)->start ^ pivot->start;
							result += (curr+19)->start ^ pivot->start;
							result += (curr+20)->start ^ pivot->start;
							result += (curr+21)->start ^ pivot->start;
							result += (curr+22)->start ^ pivot->start;
							result += (curr+23)->start ^ pivot->start;
							result += (curr+24)->start ^ pivot->start;
							result += (curr+25)->start ^ pivot->start;
							result += (curr+26)->start ^ pivot->start;
							result += (curr+27)->start ^ pivot->start;
							result += (curr+28)->start ^ pivot->start;
							result += (curr+29)->start ^ pivot->start;
							pivot++;
						}
						break;
						
					case 31:
						while ((pivot < last) && (pivot->start <= curr->end))
						{
							result += (curr+0)->start ^ pivot->start;
							result += (curr+1)->start ^ pivot->start;
							result += (curr+2)->start ^ pivot->start;
							result += (curr+3)->start ^ pivot->start;
							result += (curr+4)->start ^ pivot->start;
							result += (curr+5)->start ^ pivot->start;
							result += (curr+6)->start ^ pivot->start;
							result += (curr+7)->start ^ pivot->start;
							result += (curr+8)->start ^ pivot->start;
							result += (curr+9)->start ^ pivot->start;
							result += (curr+10)->start ^ pivot->start;
							result += (curr+11)->start ^ pivot->start;
							result += (curr+12)->start ^ pivot->start;
							result += (curr+13)->start ^ pivot->start;
							result += (curr+14)->start ^ pivot->start;
							result += (curr+15)->start ^ pivot->start;
							result += (curr+16)->start ^ pivot->start;
							result += (curr+17)->start ^ pivot->start;
							result += (curr+18)->start ^ pivot->start;
							result += (curr+19)->start ^ pivot->start;
							result += (curr+20)->start ^ pivot->start;
							result += (curr+21)->start ^ pivot->start;
							result += (curr+22)->start ^ pivot->start;
							result += (curr+23)->start ^ pivot->start;
							result += (curr+24)->start ^ pivot->start;
							result += (curr+25)->start ^ pivot->start;
							result += (curr+26)->start ^ pivot->start;
							result += (curr+27)->start ^ pivot->start;
							result += (curr+28)->start ^ pivot->start;
							result += (curr+29)->start ^ pivot->start;
							result += (curr+30)->start ^ pivot->start;
							pivot++;
						}
						break;
						
					case 32:
						while ((pivot < last) && (pivot->start <= curr->end))
						{
							result += (curr+0)->start ^ pivot->start;
							result += (curr+1)->start ^ pivot->start;
							result += (curr+2)->start ^ pivot->start;
							result += (curr+3)->start ^ pivot->start;
							result += (curr+4)->start ^ pivot->start;
							result += (curr+5)->start ^ pivot->start;
							result += (curr+6)->start ^ pivot->start;
							result += (curr+7)->start ^ pivot->start;
							result += (curr+8)->start ^ pivot->start;
							result += (curr+9)->start ^ pivot->start;
							result += (curr+10)->start ^ pivot->start;
							result += (curr+11)->start ^ pivot->start;
							result += (curr+12)->start ^ pivot->start;
							result += (curr+13)->start ^ pivot->start;
							result += (curr+14)->start ^ pivot->start;
							result += (curr+15)->start ^ pivot->start;
							result += (curr+16)->start ^ pivot->start;
							result += (curr+17)->start ^ pivot->start;
							result += (curr+18)->start ^ pivot->start;
							result += (curr+19)->start ^ pivot->start;
							result += (curr+20)->start ^ pivot->start;
							result += (curr+21)->start ^ pivot->start;
							result += (curr+22)->start ^ pivot->start;
							result += (curr+23)->start ^ pivot->start;
							result += (curr+24)->start ^ pivot->start;
							result += (curr+25)->start ^ pivot->start;
							result += (curr+26)->start ^ pivot->start;
							result += (curr+27)->start ^ pivot->start;
							result += (curr+28)->start ^ pivot->start;
							result += (curr+29)->start ^ pivot->start;
							result += (curr+30)->start ^ pivot->start;
							result += (curr+31)->start ^ pivot->start;
							pivot++;
						}
						break;
						
					default:
						while ((last-pivot >= 32) && (curr->end >= (pivot+31)->start))
						{
							for (auto k = curr; k != lastGR; k++)
							{
								result += k->start ^ (pivot+0)->start;
								result += k->start ^ (pivot+1)->start;
								result += k->start ^ (pivot+2)->start;
								result += k->start ^ (pivot+3)->start;
								result += k->start ^ (pivot+4)->start;
								result += k->start ^ (pivot+5)->start;
								result += k->start ^ (pivot+6)->start;
								result += k->start ^ (pivot+7)->start;
								result += k->start ^ (pivot+8)->start;
								result += k->start ^ (pivot+9)->start;
								result += k->start ^ (pivot+10)->start;
								result += k->start ^ (pivot+11)->start;
								result += k->start ^ (pivot+12)->start;
								result += k->start ^ (pivot+13)->start;
								result += k->start ^ (pivot+14)->start;
								result += k->start ^ (pivot+15)->start;
								result += k->start ^ (pivot+16)->start;
								result += k->start ^ (pivot+17)->start;
								result += k->start ^ (pivot+18)->start;
								result += k->start ^ (pivot+19)->start;
								result += k->start ^ (pivot+20)->start;
								result += k->start ^ (pivot+21)->start;
								result += k->start ^ (pivot+22)->start;
								result += k->start ^ (pivot+23)->start;
								result += k->start ^ (pivot+24)->start;
								result += k->start ^ (pivot+25)->start;
								result += k->start ^ (pivot+26)->start;
								result += k->start ^ (pivot+27)->start;
								result += k->start ^ (pivot+28)->start;
								result += k->start ^ (pivot+29)->start;
								result += k->start ^ (pivot+30)->start;
								result += k->start ^ (pivot+31)->start;
							}
							pivot += 32;
						}
						
						if ((last-pivot >= 16) && (curr->end >= (pivot+15)->start))
						{
							for (auto k = curr; k != lastGR; k++)
							{
								result += k->start ^ (pivot+0)->start;
								result += k->start ^ (pivot+1)->start;
								result += k->start ^ (pivot+2)->start;
								result += k->start ^ (pivot+3)->start;
								result += k->start ^ (pivot+4)->start;
								result += k->start ^ (pivot+5)->start;
								result += k->start ^ (pivot+6)->start;
								result += k->start ^ (pivot+7)->start;
								result += k->start ^ (pivot+8)->start;
								result += k->start ^ (pivot+9)->start;
								result += k->start ^ (pivot+10)->start;
								result += k->start ^ (pivot+11)->start;
								result += k->start ^ (pivot+12)->start;
								result += k->start ^ (pivot+13)->start;
								result += k->start ^ (pivot+14)->start;
								result += k->start ^ (pivot+15)->start;
							}
							pivot += 16;
						}
						
						if ((last-pivot >= 8) && (curr->end >= (pivot+7)->start))
						{
							for (auto k = curr; k != lastGR; k++)
							{
								result += k->start ^ (pivot+0)->start;
								result += k->start ^ (pivot+1)->start;
								result += k->start ^ (pivot+2)->start;
								result += k->start ^ (pivot+3)->start;
								result += k->start ^ (pivot+4)->start;
								result += k->start ^ (pivot+5)->start;
								result += k->start ^ (pivot+6)->start;
								result += k->start ^ (pivot+7)->start;
							}
							pivot += 8;
						}
						
						if ((last-pivot >= 4) && (curr->end >= (pivot+3)->start))
						{
							for (auto k = curr; k != lastGR; k++)
							{
								result += k->start ^ (pivot+0)->start;
								result += k->start ^ (pivot+1)->start;
								result += k->start ^ (pivot+2)->start;
								result += k->start ^ (pivot+3)->start;
							}
							pivot += 4;
						}
						
						while ((pivot < last) && (pivot->start <= curr->end))
						{
							auto k = curr;
							while (lastGR-k >= 32)
							{
								result += (k+0)->start ^ pivot->start;
								result += (k+1)->start ^ pivot->start;
								result += (k+2)->start ^ pivot->start;
								result += (k+3)->start ^ pivot->start;
								result += (k+4)->start ^ pivot->start;
								result += (k+5)->start ^ pivot->start;
								result += (k+6)->start ^ pivot->start;
								result += (k+7)->start ^ pivot->start;
								result += (k+8)->start ^ pivot->start;
								result += (k+9)->start ^ pivot->start;
								result += (k+10)->start ^ pivot->start;
								result += (k+11)->start ^ pivot->start;
								result += (k+12)->start ^ pivot->start;
								result += (k+13)->start ^ pivot->start;
								result += (k+14)->start ^ pivot->start;
								result += (k+15)->start ^ pivot->start;
								result += (k+16)->start ^ pivot->start;
								result += (k+17)->start ^ pivot->start;
								result += (k+18)->start ^ pivot->start;
								result += (k+19)->start ^ pivot->start;
								result += (k+20)->start ^ pivot->start;
								result += (k+21)->start ^ pivot->start;
								result += (k+22)->start ^ pivot->start;
								result += (k+23)->start ^ pivot->start;
								result += (k+24)->start ^ pivot->start;
								result += (k+25)->start ^ pivot->start;
								result += (k+26)->start ^ pivot->start;
								result += (k+27)->start ^ pivot->start;
								result += (k+28)->start ^ pivot->start;
								result += (k+29)->start ^ pivot->start;
								result += (k+30)->start ^ pivot->start;
								result += (k+31)->start ^ pivot->start;
								k += 32;
							}
							
							if (lastGR-k >= 16)
							{
								result += (k+0)->start ^ pivot->start;
								result += (k+1)->start ^ pivot->start;
								result += (k+2)->start ^ pivot->start;
								result += (k+3)->start ^ pivot->start;
								result += (k+4)->start ^ pivot->start;
								result += (k+5)->start ^ pivot->start;
								result += (k+6)->start ^ pivot->start;
								result += (k+7)->start ^ pivot->start;
								result += (k+8)->start ^ pivot->start;
								result += (k+9)->start ^ pivot->start;
								result += (k+10)->start ^ pivot->start;
								result += (k+11)->start ^ pivot->start;
								result += (k+12)->start ^ pivot->start;
								result += (k+13)->start ^ pivot->start;
								result += (k+14)->start ^ pivot->start;
								result += (k+15)->start ^ pivot->start;
								k += 16;
							}
							
							if (lastGR-k >= 8)
							{
								result += (k+0)->start ^ pivot->start;
								result += (k+1)->start ^ pivot->start;
								result += (k+2)->start ^ pivot->start;
								result += (k+3)->start ^ pivot->start;
								result += (k+4)->start ^ pivot->start;
								result += (k+5)->start ^ pivot->start;
								result += (k+6)->start ^ pivot->start;
								result += (k+7)->start ^ pivot->start;
								k += 8;
							}
							
							if (lastGR-k >= 4)
							{
								result += (k+0)->start ^ pivot->start;
								result += (k+1)->start ^ pivot->start;
								result += (k+2)->start ^ pivot->start;
								result += (k+3)->start ^ pivot->start;
								k += 4;
							}
							
							while (k != lastGR)
							{
								result += k->start ^ pivot->start; k++;
							}
							
							pivot++;
						}
						break;
				}
			}
			GR.clear();
		}
		else
		{
			//step 1: gather group for S
			while ((s < lastS) && (r->start >= s->start))
			{
				GS.emplace_back(s->start, s->end);
				s++;
			}
			
			//sort group by end point
			GS.sortByEnd();
			
			
			//step 2: run internal loop
			auto pivot = r;
			auto lastGS = GS.end();
			for (auto curs = GS.begin(); curs != lastGS; curs++)
			{
				auto bufferSize = lastGS-curs;
				
				if (pivot == lastR)
					break;
				if (curs->end < R.minStart)
					continue;
				
				cbucket_id = ceil((double)(curs->end-R.minStart)/BIR.bucket_range);
				if (cbucket_id >= BIR.numBuckets)
					cbucket_id = BIR.numBuckets-1;
				pbucket_id = ceil((double)(pivot->end-R.minStart)/BIR.bucket_range);
				if (pbucket_id >= BIR.numBuckets)
					pbucket_id = BIR.numBuckets-1;
				
				if (cbucket_id > pbucket_id)
				{
					auto last = BIR[cbucket_id-1].last;
					switch (bufferSize)
					{
						case 1:
							while (last-pivot >= 32)
							{
								result += (pivot+0)->start ^ curs->start;
								result += (pivot+1)->start ^ curs->start;
								result += (pivot+2)->start ^ curs->start;
								result += (pivot+3)->start ^ curs->start;
								result += (pivot+4)->start ^ curs->start;
								result += (pivot+5)->start ^ curs->start;
								result += (pivot+6)->start ^ curs->start;
								result += (pivot+7)->start ^ curs->start;
								result += (pivot+8)->start ^ curs->start;
								result += (pivot+9)->start ^ curs->start;
								result += (pivot+10)->start ^ curs->start;
								result += (pivot+11)->start ^ curs->start;
								result += (pivot+12)->start ^ curs->start;
								result += (pivot+13)->start ^ curs->start;
								result += (pivot+14)->start ^ curs->start;
								result += (pivot+15)->start ^ curs->start;
								result += (pivot+16)->start ^ curs->start;
								result += (pivot+17)->start ^ curs->start;
								result += (pivot+18)->start ^ curs->start;
								result += (pivot+19)->start ^ curs->start;
								result += (pivot+20)->start ^ curs->start;
								result += (pivot+21)->start ^ curs->start;
								result += (pivot+22)->start ^ curs->start;
								result += (pivot+23)->start ^ curs->start;
								result += (pivot+24)->start ^ curs->start;
								result += (pivot+25)->start ^ curs->start;
								result += (pivot+26)->start ^ curs->start;
								result += (pivot+27)->start ^ curs->start;
								result += (pivot+28)->start ^ curs->start;
								result += (pivot+29)->start ^ curs->start;
								result += (pivot+30)->start ^ curs->start;
								result += (pivot+31)->start ^ curs->start;
								pivot += 32;
							}
							
							while (pivot < last)
							{
								result += pivot->start ^ curs->start;
								pivot++;
							}
							break;
							
						case 2:
							while (last-pivot >= 16)
							{
								result += (pivot+0)->start ^ (curs+0)->start;
								result += (pivot+0)->start ^ (curs+1)->start;
								result += (pivot+1)->start ^ (curs+0)->start;
								result += (pivot+1)->start ^ (curs+1)->start;
								result += (pivot+2)->start ^ (curs+0)->start;
								result += (pivot+2)->start ^ (curs+1)->start;
								result += (pivot+3)->start ^ (curs+0)->start;
								result += (pivot+3)->start ^ (curs+1)->start;
								result += (pivot+4)->start ^ (curs+0)->start;
								result += (pivot+4)->start ^ (curs+1)->start;
								result += (pivot+5)->start ^ (curs+0)->start;
								result += (pivot+5)->start ^ (curs+1)->start;
								result += (pivot+6)->start ^ (curs+0)->start;
								result += (pivot+6)->start ^ (curs+1)->start;
								result += (pivot+7)->start ^ (curs+0)->start;
								result += (pivot+7)->start ^ (curs+1)->start;
								result += (pivot+8)->start ^ (curs+0)->start;
								result += (pivot+8)->start ^ (curs+1)->start;
								result += (pivot+9)->start ^ (curs+0)->start;
								result += (pivot+9)->start ^ (curs+1)->start;
								result += (pivot+10)->start ^ (curs+0)->start;
								result += (pivot+10)->start ^ (curs+1)->start;
								result += (pivot+11)->start ^ (curs+0)->start;
								result += (pivot+11)->start ^ (curs+1)->start;
								result += (pivot+12)->start ^ (curs+0)->start;
								result += (pivot+12)->start ^ (curs+1)->start;
								result += (pivot+13)->start ^ (curs+0)->start;
								result += (pivot+13)->start ^ (curs+1)->start;
								result += (pivot+14)->start ^ (curs+0)->start;
								result += (pivot+14)->start ^ (curs+1)->start;
								result += (pivot+15)->start ^ (curs+0)->start;
								result += (pivot+15)->start ^ (curs+1)->start;
								pivot += 16;
							}
							
							while (pivot < last)
							{
								result += pivot->start ^ (curs+0)->start;
								result += pivot->start ^ (curs+1)->start;
								pivot++;
							}
							
							break;
							
						case 3:
							while (last-pivot >= 8)
							{
								result += (pivot+0)->start ^ (curs+0)->start;
								result += (pivot+0)->start ^ (curs+1)->start;
								result += (pivot+0)->start ^ (curs+2)->start;
								result += (pivot+1)->start ^ (curs+0)->start;
								result += (pivot+1)->start ^ (curs+1)->start;
								result += (pivot+1)->start ^ (curs+2)->start;
								result += (pivot+2)->start ^ (curs+0)->start;
								result += (pivot+2)->start ^ (curs+1)->start;
								result += (pivot+2)->start ^ (curs+2)->start;
								result += (pivot+3)->start ^ (curs+0)->start;
								result += (pivot+3)->start ^ (curs+1)->start;
								result += (pivot+3)->start ^ (curs+2)->start;
								result += (pivot+4)->start ^ (curs+0)->start;
								result += (pivot+4)->start ^ (curs+1)->start;
								result += (pivot+4)->start ^ (curs+2)->start;
								result += (pivot+5)->start ^ (curs+0)->start;
								result += (pivot+5)->start ^ (curs+1)->start;
								result += (pivot+5)->start ^ (curs+2)->start;
								result += (pivot+6)->start ^ (curs+0)->start;
								result += (pivot+6)->start ^ (curs+1)->start;
								result += (pivot+6)->start ^ (curs+2)->start;
								result += (pivot+7)->start ^ (curs+0)->start;
								result += (pivot+7)->start ^ (curs+1)->start;
								result += (pivot+7)->start ^ (curs+2)->start;
								pivot += 8;
							}
							
							while (pivot < last)
							{
								result += pivot->start ^ (curs+0)->start;
								result += pivot->start ^ (curs+1)->start;
								result += pivot->start ^ (curs+2)->start;
								pivot++;
							}
							break;
							
						case 4:
							while (last-pivot >= 8)
							{
								result += (pivot+0)->start ^ (curs+0)->start;
								result += (pivot+0)->start ^ (curs+1)->start;
								result += (pivot+0)->start ^ (curs+2)->start;
								result += (pivot+0)->start ^ (curs+3)->start;
								result += (pivot+1)->start ^ (curs+0)->start;
								result += (pivot+1)->start ^ (curs+1)->start;
								result += (pivot+1)->start ^ (curs+2)->start;
								result += (pivot+1)->start ^ (curs+3)->start;
								result += (pivot+2)->start ^ (curs+0)->start;
								result += (pivot+2)->start ^ (curs+1)->start;
								result += (pivot+2)->start ^ (curs+2)->start;
								result += (pivot+2)->start ^ (curs+3)->start;
								result += (pivot+3)->start ^ (curs+0)->start;
								result += (pivot+3)->start ^ (curs+1)->start;
								result += (pivot+3)->start ^ (curs+2)->start;
								result += (pivot+3)->start ^ (curs+3)->start;
								result += (pivot+4)->start ^ (curs+0)->start;
								result += (pivot+4)->start ^ (curs+1)->start;
								result += (pivot+4)->start ^ (curs+2)->start;
								result += (pivot+4)->start ^ (curs+3)->start;
								result += (pivot+5)->start ^ (curs+0)->start;
								result += (pivot+5)->start ^ (curs+1)->start;
								result += (pivot+5)->start ^ (curs+2)->start;
								result += (pivot+5)->start ^ (curs+3)->start;
								result += (pivot+6)->start ^ (curs+0)->start;
								result += (pivot+6)->start ^ (curs+1)->start;
								result += (pivot+6)->start ^ (curs+2)->start;
								result += (pivot+6)->start ^ (curs+3)->start;
								result += (pivot+7)->start ^ (curs+0)->start;
								result += (pivot+7)->start ^ (curs+1)->start;
								result += (pivot+7)->start ^ (curs+2)->start;
								result += (pivot+7)->start ^ (curs+3)->start;
								pivot += 8;
							}
							
							while (pivot < last)
							{
								result += pivot->start ^ (curs+0)->start;
								result += pivot->start ^ (curs+1)->start;
								result += pivot->start ^ (curs+2)->start;
								result += pivot->start ^ (curs+3)->start;
								pivot++;
							}
							break;
							
						case 5:
							while (pivot < last)
							{
								result += pivot->start ^ (curs+0)->start;
								result += pivot->start ^ (curs+1)->start;
								result += pivot->start ^ (curs+2)->start;
								result += pivot->start ^ (curs+3)->start;
								result += pivot->start ^ (curs+4)->start;
								pivot++;
							}
							break;
							
						case 6:
							while (pivot < last)
							{
								result += pivot->start ^ (curs+0)->start;
								result += pivot->start ^ (curs+1)->start;
								result += pivot->start ^ (curs+2)->start;
								result += pivot->start ^ (curs+3)->start;
								result += pivot->start ^ (curs+4)->start;
								result += pivot->start ^ (curs+5)->start;
								pivot++;
							}
							break;
							
						case 7:
							while (pivot < last)
							{
								result += pivot->start ^ (curs+0)->start;
								result += pivot->start ^ (curs+1)->start;
								result += pivot->start ^ (curs+2)->start;
								result += pivot->start ^ (curs+3)->start;
								result += pivot->start ^ (curs+4)->start;
								result += pivot->start ^ (curs+5)->start;
								result += pivot->start ^ (curs+6)->start;
								pivot++;
							}
							break;
							
						case 8:
							while (pivot < last)
							{
								result += pivot->start ^ (curs+0)->start;
								result += pivot->start ^ (curs+1)->start;
								result += pivot->start ^ (curs+2)->start;
								result += pivot->start ^ (curs+3)->start;
								result += pivot->start ^ (curs+4)->start;
								result += pivot->start ^ (curs+5)->start;
								result += pivot->start ^ (curs+6)->start;
								result += pivot->start ^ (curs+7)->start;
								pivot++;
							}
							break;
							
						case 9:
							while (pivot < last)
							{
								result += pivot->start ^ (curs+0)->start;
								result += pivot->start ^ (curs+1)->start;
								result += pivot->start ^ (curs+2)->start;
								result += pivot->start ^ (curs+3)->start;
								result += pivot->start ^ (curs+4)->start;
								result += pivot->start ^ (curs+5)->start;
								result += pivot->start ^ (curs+6)->start;
								result += pivot->start ^ (curs+7)->start;
								result += pivot->start ^ (curs+8)->start;
								pivot++;
							}
							break;
							
						case 10:
							while (pivot < last)
							{
								result += pivot->start ^ (curs+0)->start;
								result += pivot->start ^ (curs+1)->start;
								result += pivot->start ^ (curs+2)->start;
								result += pivot->start ^ (curs+3)->start;
								result += pivot->start ^ (curs+4)->start;
								result += pivot->start ^ (curs+5)->start;
								result += pivot->start ^ (curs+6)->start;
								result += pivot->start ^ (curs+7)->start;
								result += pivot->start ^ (curs+8)->start;
								result += pivot->start ^ (curs+9)->start;
								pivot++;
							}
							break;
							
						case 11:
							while (pivot < last)
							{
								result += pivot->start ^ (curs+0)->start;
								result += pivot->start ^ (curs+1)->start;
								result += pivot->start ^ (curs+2)->start;
								result += pivot->start ^ (curs+3)->start;
								result += pivot->start ^ (curs+4)->start;
								result += pivot->start ^ (curs+5)->start;
								result += pivot->start ^ (curs+6)->start;
								result += pivot->start ^ (curs+7)->start;
								result += pivot->start ^ (curs+8)->start;
								result += pivot->start ^ (curs+9)->start;
								result += pivot->start ^ (curs+10)->start;
								pivot++;
							}
							break;
							
						case 12:
							while (pivot < last)
							{
								result += pivot->start ^ (curs+0)->start;
								result += pivot->start ^ (curs+1)->start;
								result += pivot->start ^ (curs+2)->start;
								result += pivot->start ^ (curs+3)->start;
								result += pivot->start ^ (curs+4)->start;
								result += pivot->start ^ (curs+5)->start;
								result += pivot->start ^ (curs+6)->start;
								result += pivot->start ^ (curs+7)->start;
								result += pivot->start ^ (curs+8)->start;
								result += pivot->start ^ (curs+9)->start;
								result += pivot->start ^ (curs+10)->start;
								result += pivot->start ^ (curs+11)->start;
								pivot++;
							}
							break;
							
						case 13:
							while (pivot < last)
							{
								result += pivot->start ^ (curs+0)->start;
								result += pivot->start ^ (curs+1)->start;
								result += pivot->start ^ (curs+2)->start;
								result += pivot->start ^ (curs+3)->start;
								result += pivot->start ^ (curs+4)->start;
								result += pivot->start ^ (curs+5)->start;
								result += pivot->start ^ (curs+6)->start;
								result += pivot->start ^ (curs+7)->start;
								result += pivot->start ^ (curs+8)->start;
								result += pivot->start ^ (curs+9)->start;
								result += pivot->start ^ (curs+10)->start;
								result += pivot->start ^ (curs+11)->start;
								result += pivot->start ^ (curs+12)->start;
								pivot++;
							}
							break;
							
						case 14:
							while (pivot < last)
							{
								result += pivot->start ^ (curs+0)->start;
								result += pivot->start ^ (curs+1)->start;
								result += pivot->start ^ (curs+2)->start;
								result += pivot->start ^ (curs+3)->start;
								result += pivot->start ^ (curs+4)->start;
								result += pivot->start ^ (curs+5)->start;
								result += pivot->start ^ (curs+6)->start;
								result += pivot->start ^ (curs+7)->start;
								result += pivot->start ^ (curs+8)->start;
								result += pivot->start ^ (curs+9)->start;
								result += pivot->start ^ (curs+10)->start;
								result += pivot->start ^ (curs+11)->start;
								result += pivot->start ^ (curs+12)->start;
								result += pivot->start ^ (curs+13)->start;
								pivot++;
							}
							break;
							
						case 15:
							while (pivot < last)
							{
								result += pivot->start ^ (curs+0)->start;
								result += pivot->start ^ (curs+1)->start;
								result += pivot->start ^ (curs+2)->start;
								result += pivot->start ^ (curs+3)->start;
								result += pivot->start ^ (curs+4)->start;
								result += pivot->start ^ (curs+5)->start;
								result += pivot->start ^ (curs+6)->start;
								result += pivot->start ^ (curs+7)->start;
								result += pivot->start ^ (curs+8)->start;
								result += pivot->start ^ (curs+9)->start;
								result += pivot->start ^ (curs+10)->start;
								result += pivot->start ^ (curs+11)->start;
								result += pivot->start ^ (curs+12)->start;
								result += pivot->start ^ (curs+13)->start;
								result += pivot->start ^ (curs+14)->start;
								pivot++;
							}
							break;
							
						case 16:
							while (pivot < last)
							{
								result += pivot->start ^ (curs+0)->start;
								result += pivot->start ^ (curs+1)->start;
								result += pivot->start ^ (curs+2)->start;
								result += pivot->start ^ (curs+3)->start;
								result += pivot->start ^ (curs+4)->start;
								result += pivot->start ^ (curs+5)->start;
								result += pivot->start ^ (curs+6)->start;
								result += pivot->start ^ (curs+7)->start;
								result += pivot->start ^ (curs+8)->start;
								result += pivot->start ^ (curs+9)->start;
								result += pivot->start ^ (curs+10)->start;
								result += pivot->start ^ (curs+11)->start;
								result += pivot->start ^ (curs+12)->start;
								result += pivot->start ^ (curs+13)->start;
								result += pivot->start ^ (curs+14)->start;
								result += pivot->start ^ (curs+15)->start;
								pivot++;
							}
							break;
							
						case 17:
							while (pivot < last)
							{
								result += pivot->start ^ (curs+0)->start;
								result += pivot->start ^ (curs+1)->start;
								result += pivot->start ^ (curs+2)->start;
								result += pivot->start ^ (curs+3)->start;
								result += pivot->start ^ (curs+4)->start;
								result += pivot->start ^ (curs+5)->start;
								result += pivot->start ^ (curs+6)->start;
								result += pivot->start ^ (curs+7)->start;
								result += pivot->start ^ (curs+8)->start;
								result += pivot->start ^ (curs+9)->start;
								result += pivot->start ^ (curs+10)->start;
								result += pivot->start ^ (curs+11)->start;
								result += pivot->start ^ (curs+12)->start;
								result += pivot->start ^ (curs+13)->start;
								result += pivot->start ^ (curs+14)->start;
								result += pivot->start ^ (curs+15)->start;
								result += pivot->start ^ (curs+16)->start;
								pivot++;
							}
							break;
							
						case 18:
							while (pivot < last)
							{
								result += pivot->start ^ (curs+0)->start;
								result += pivot->start ^ (curs+1)->start;
								result += pivot->start ^ (curs+2)->start;
								result += pivot->start ^ (curs+3)->start;
								result += pivot->start ^ (curs+4)->start;
								result += pivot->start ^ (curs+5)->start;
								result += pivot->start ^ (curs+6)->start;
								result += pivot->start ^ (curs+7)->start;
								result += pivot->start ^ (curs+8)->start;
								result += pivot->start ^ (curs+9)->start;
								result += pivot->start ^ (curs+10)->start;
								result += pivot->start ^ (curs+11)->start;
								result += pivot->start ^ (curs+12)->start;
								result += pivot->start ^ (curs+13)->start;
								result += pivot->start ^ (curs+14)->start;
								result += pivot->start ^ (curs+15)->start;
								result += pivot->start ^ (curs+16)->start;
								result += pivot->start ^ (curs+17)->start;
								pivot++;
							}
							break;
							
						case 19:
							while (pivot < last)
							{
								result += pivot->start ^ (curs+0)->start;
								result += pivot->start ^ (curs+1)->start;
								result += pivot->start ^ (curs+2)->start;
								result += pivot->start ^ (curs+3)->start;
								result += pivot->start ^ (curs+4)->start;
								result += pivot->start ^ (curs+5)->start;
								result += pivot->start ^ (curs+6)->start;
								result += pivot->start ^ (curs+7)->start;
								result += pivot->start ^ (curs+8)->start;
								result += pivot->start ^ (curs+9)->start;
								result += pivot->start ^ (curs+10)->start;
								result += pivot->start ^ (curs+11)->start;
								result += pivot->start ^ (curs+12)->start;
								result += pivot->start ^ (curs+13)->start;
								result += pivot->start ^ (curs+14)->start;
								result += pivot->start ^ (curs+15)->start;
								result += pivot->start ^ (curs+16)->start;
								result += pivot->start ^ (curs+17)->start;
								result += pivot->start ^ (curs+18)->start;
								pivot++;
							}
							break;
							
						case 20:
							while (pivot < last)
							{
								result += pivot->start ^ (curs+0)->start;
								result += pivot->start ^ (curs+1)->start;
								result += pivot->start ^ (curs+2)->start;
								result += pivot->start ^ (curs+3)->start;
								result += pivot->start ^ (curs+4)->start;
								result += pivot->start ^ (curs+5)->start;
								result += pivot->start ^ (curs+6)->start;
								result += pivot->start ^ (curs+7)->start;
								result += pivot->start ^ (curs+8)->start;
								result += pivot->start ^ (curs+9)->start;
								result += pivot->start ^ (curs+10)->start;
								result += pivot->start ^ (curs+11)->start;
								result += pivot->start ^ (curs+12)->start;
								result += pivot->start ^ (curs+13)->start;
								result += pivot->start ^ (curs+14)->start;
								result += pivot->start ^ (curs+15)->start;
								result += pivot->start ^ (curs+16)->start;
								result += pivot->start ^ (curs+17)->start;
								result += pivot->start ^ (curs+18)->start;
								result += pivot->start ^ (curs+19)->start;
								pivot++;
							}
							break;
							
						case 21:
							while (pivot < last)
							{
								result += pivot->start ^ (curs+0)->start;
								result += pivot->start ^ (curs+1)->start;
								result += pivot->start ^ (curs+2)->start;
								result += pivot->start ^ (curs+3)->start;
								result += pivot->start ^ (curs+4)->start;
								result += pivot->start ^ (curs+5)->start;
								result += pivot->start ^ (curs+6)->start;
								result += pivot->start ^ (curs+7)->start;
								result += pivot->start ^ (curs+8)->start;
								result += pivot->start ^ (curs+9)->start;
								result += pivot->start ^ (curs+10)->start;
								result += pivot->start ^ (curs+11)->start;
								result += pivot->start ^ (curs+12)->start;
								result += pivot->start ^ (curs+13)->start;
								result += pivot->start ^ (curs+14)->start;
								result += pivot->start ^ (curs+15)->start;
								result += pivot->start ^ (curs+16)->start;
								result += pivot->start ^ (curs+17)->start;
								result += pivot->start ^ (curs+18)->start;
								result += pivot->start ^ (curs+19)->start;
								result += pivot->start ^ (curs+20)->start;
								pivot++;
							}
							break;
							
						case 22:
							while (pivot < last)
							{
								result += pivot->start ^ (curs+0)->start;
								result += pivot->start ^ (curs+1)->start;
								result += pivot->start ^ (curs+2)->start;
								result += pivot->start ^ (curs+3)->start;
								result += pivot->start ^ (curs+4)->start;
								result += pivot->start ^ (curs+5)->start;
								result += pivot->start ^ (curs+6)->start;
								result += pivot->start ^ (curs+7)->start;
								result += pivot->start ^ (curs+8)->start;
								result += pivot->start ^ (curs+9)->start;
								result += pivot->start ^ (curs+10)->start;
								result += pivot->start ^ (curs+11)->start;
								result += pivot->start ^ (curs+12)->start;
								result += pivot->start ^ (curs+13)->start;
								result += pivot->start ^ (curs+14)->start;
								result += pivot->start ^ (curs+15)->start;
								result += pivot->start ^ (curs+16)->start;
								result += pivot->start ^ (curs+17)->start;
								result += pivot->start ^ (curs+18)->start;
								result += pivot->start ^ (curs+19)->start;
								result += pivot->start ^ (curs+20)->start;
								result += pivot->start ^ (curs+21)->start;
								pivot++;
							}
							break;
							
						case 23:
							while (pivot < last)
							{
								result += pivot->start ^ (curs+0)->start;
								result += pivot->start ^ (curs+1)->start;
								result += pivot->start ^ (curs+2)->start;
								result += pivot->start ^ (curs+3)->start;
								result += pivot->start ^ (curs+4)->start;
								result += pivot->start ^ (curs+5)->start;
								result += pivot->start ^ (curs+6)->start;
								result += pivot->start ^ (curs+7)->start;
								result += pivot->start ^ (curs+8)->start;
								result += pivot->start ^ (curs+9)->start;
								result += pivot->start ^ (curs+10)->start;
								result += pivot->start ^ (curs+11)->start;
								result += pivot->start ^ (curs+12)->start;
								result += pivot->start ^ (curs+13)->start;
								result += pivot->start ^ (curs+14)->start;
								result += pivot->start ^ (curs+15)->start;
								result += pivot->start ^ (curs+16)->start;
								result += pivot->start ^ (curs+17)->start;
								result += pivot->start ^ (curs+18)->start;
								result += pivot->start ^ (curs+19)->start;
								result += pivot->start ^ (curs+20)->start;
								result += pivot->start ^ (curs+21)->start;
								result += pivot->start ^ (curs+22)->start;
								pivot++;
							}
							break;
							
						case 24:
							while (pivot < last)
							{
								result += pivot->start ^ (curs+0)->start;
								result += pivot->start ^ (curs+1)->start;
								result += pivot->start ^ (curs+2)->start;
								result += pivot->start ^ (curs+3)->start;
								result += pivot->start ^ (curs+4)->start;
								result += pivot->start ^ (curs+5)->start;
								result += pivot->start ^ (curs+6)->start;
								result += pivot->start ^ (curs+7)->start;
								result += pivot->start ^ (curs+8)->start;
								result += pivot->start ^ (curs+9)->start;
								result += pivot->start ^ (curs+10)->start;
								result += pivot->start ^ (curs+11)->start;
								result += pivot->start ^ (curs+12)->start;
								result += pivot->start ^ (curs+13)->start;
								result += pivot->start ^ (curs+14)->start;
								result += pivot->start ^ (curs+15)->start;
								result += pivot->start ^ (curs+16)->start;
								result += pivot->start ^ (curs+17)->start;
								result += pivot->start ^ (curs+18)->start;
								result += pivot->start ^ (curs+19)->start;
								result += pivot->start ^ (curs+20)->start;
								result += pivot->start ^ (curs+21)->start;
								result += pivot->start ^ (curs+22)->start;
								result += pivot->start ^ (curs+23)->start;
								pivot++;
							}
							break;
							
						case 25:
							while (pivot < last)
							{
								result += pivot->start ^ (curs+0)->start;
								result += pivot->start ^ (curs+1)->start;
								result += pivot->start ^ (curs+2)->start;
								result += pivot->start ^ (curs+3)->start;
								result += pivot->start ^ (curs+4)->start;
								result += pivot->start ^ (curs+5)->start;
								result += pivot->start ^ (curs+6)->start;
								result += pivot->start ^ (curs+7)->start;
								result += pivot->start ^ (curs+8)->start;
								result += pivot->start ^ (curs+9)->start;
								result += pivot->start ^ (curs+10)->start;
								result += pivot->start ^ (curs+11)->start;
								result += pivot->start ^ (curs+12)->start;
								result += pivot->start ^ (curs+13)->start;
								result += pivot->start ^ (curs+14)->start;
								result += pivot->start ^ (curs+15)->start;
								result += pivot->start ^ (curs+16)->start;
								result += pivot->start ^ (curs+17)->start;
								result += pivot->start ^ (curs+18)->start;
								result += pivot->start ^ (curs+19)->start;
								result += pivot->start ^ (curs+20)->start;
								result += pivot->start ^ (curs+21)->start;
								result += pivot->start ^ (curs+22)->start;
								result += pivot->start ^ (curs+23)->start;
								result += pivot->start ^ (curs+24)->start;
								pivot++;
							}
							break;
							
						case 26:
							while (pivot < last)
							{
								result += pivot->start ^ (curs+0)->start;
								result += pivot->start ^ (curs+1)->start;
								result += pivot->start ^ (curs+2)->start;
								result += pivot->start ^ (curs+3)->start;
								result += pivot->start ^ (curs+4)->start;
								result += pivot->start ^ (curs+5)->start;
								result += pivot->start ^ (curs+6)->start;
								result += pivot->start ^ (curs+7)->start;
								result += pivot->start ^ (curs+8)->start;
								result += pivot->start ^ (curs+9)->start;
								result += pivot->start ^ (curs+10)->start;
								result += pivot->start ^ (curs+11)->start;
								result += pivot->start ^ (curs+12)->start;
								result += pivot->start ^ (curs+13)->start;
								result += pivot->start ^ (curs+14)->start;
								result += pivot->start ^ (curs+15)->start;
								result += pivot->start ^ (curs+16)->start;
								result += pivot->start ^ (curs+17)->start;
								result += pivot->start ^ (curs+18)->start;
								result += pivot->start ^ (curs+19)->start;
								result += pivot->start ^ (curs+20)->start;
								result += pivot->start ^ (curs+21)->start;
								result += pivot->start ^ (curs+22)->start;
								result += pivot->start ^ (curs+23)->start;
								result += pivot->start ^ (curs+24)->start;
								result += pivot->start ^ (curs+25)->start;
								pivot++;
							}
							break;
							
						case 27:
							while (pivot < last)
							{
								result += pivot->start ^ (curs+0)->start;
								result += pivot->start ^ (curs+1)->start;
								result += pivot->start ^ (curs+2)->start;
								result += pivot->start ^ (curs+3)->start;
								result += pivot->start ^ (curs+4)->start;
								result += pivot->start ^ (curs+5)->start;
								result += pivot->start ^ (curs+6)->start;
								result += pivot->start ^ (curs+7)->start;
								result += pivot->start ^ (curs+8)->start;
								result += pivot->start ^ (curs+9)->start;
								result += pivot->start ^ (curs+10)->start;
								result += pivot->start ^ (curs+11)->start;
								result += pivot->start ^ (curs+12)->start;
								result += pivot->start ^ (curs+13)->start;
								result += pivot->start ^ (curs+14)->start;
								result += pivot->start ^ (curs+15)->start;
								result += pivot->start ^ (curs+16)->start;
								result += pivot->start ^ (curs+17)->start;
								result += pivot->start ^ (curs+18)->start;
								result += pivot->start ^ (curs+19)->start;
								result += pivot->start ^ (curs+20)->start;
								result += pivot->start ^ (curs+21)->start;
								result += pivot->start ^ (curs+22)->start;
								result += pivot->start ^ (curs+23)->start;
								result += pivot->start ^ (curs+24)->start;
								result += pivot->start ^ (curs+25)->start;
								result += pivot->start ^ (curs+26)->start;
								pivot++;
							}
							break;
							
						case 28:
							while (pivot < last)
							{
								result += pivot->start ^ (curs+0)->start;
								result += pivot->start ^ (curs+1)->start;
								result += pivot->start ^ (curs+2)->start;
								result += pivot->start ^ (curs+3)->start;
								result += pivot->start ^ (curs+4)->start;
								result += pivot->start ^ (curs+5)->start;
								result += pivot->start ^ (curs+6)->start;
								result += pivot->start ^ (curs+7)->start;
								result += pivot->start ^ (curs+8)->start;
								result += pivot->start ^ (curs+9)->start;
								result += pivot->start ^ (curs+10)->start;
								result += pivot->start ^ (curs+11)->start;
								result += pivot->start ^ (curs+12)->start;
								result += pivot->start ^ (curs+13)->start;
								result += pivot->start ^ (curs+14)->start;
								result += pivot->start ^ (curs+15)->start;
								result += pivot->start ^ (curs+16)->start;
								result += pivot->start ^ (curs+17)->start;
								result += pivot->start ^ (curs+18)->start;
								result += pivot->start ^ (curs+19)->start;
								result += pivot->start ^ (curs+20)->start;
								result += pivot->start ^ (curs+21)->start;
								result += pivot->start ^ (curs+22)->start;
								result += pivot->start ^ (curs+23)->start;
								result += pivot->start ^ (curs+24)->start;
								result += pivot->start ^ (curs+25)->start;
								result += pivot->start ^ (curs+26)->start;
								result += pivot->start ^ (curs+27)->start;
								pivot++;
							}
							break;
							
						case 29:
							while (pivot < last)
							{
								result += pivot->start ^ (curs+0)->start;
								result += pivot->start ^ (curs+1)->start;
								result += pivot->start ^ (curs+2)->start;
								result += pivot->start ^ (curs+3)->start;
								result += pivot->start ^ (curs+4)->start;
								result += pivot->start ^ (curs+5)->start;
								result += pivot->start ^ (curs+6)->start;
								result += pivot->start ^ (curs+7)->start;
								result += pivot->start ^ (curs+8)->start;
								result += pivot->start ^ (curs+9)->start;
								result += pivot->start ^ (curs+10)->start;
								result += pivot->start ^ (curs+11)->start;
								result += pivot->start ^ (curs+12)->start;
								result += pivot->start ^ (curs+13)->start;
								result += pivot->start ^ (curs+14)->start;
								result += pivot->start ^ (curs+15)->start;
								result += pivot->start ^ (curs+16)->start;
								result += pivot->start ^ (curs+17)->start;
								result += pivot->start ^ (curs+18)->start;
								result += pivot->start ^ (curs+19)->start;
								result += pivot->start ^ (curs+20)->start;
								result += pivot->start ^ (curs+21)->start;
								result += pivot->start ^ (curs+22)->start;
								result += pivot->start ^ (curs+23)->start;
								result += pivot->start ^ (curs+24)->start;
								result += pivot->start ^ (curs+25)->start;
								result += pivot->start ^ (curs+26)->start;
								result += pivot->start ^ (curs+27)->start;
								result += pivot->start ^ (curs+28)->start;
								pivot++;
							}
							break;
							
						case 30:
							while (pivot < last)
							{
								result += pivot->start ^ (curs+0)->start;
								result += pivot->start ^ (curs+1)->start;
								result += pivot->start ^ (curs+2)->start;
								result += pivot->start ^ (curs+3)->start;
								result += pivot->start ^ (curs+4)->start;
								result += pivot->start ^ (curs+5)->start;
								result += pivot->start ^ (curs+6)->start;
								result += pivot->start ^ (curs+7)->start;
								result += pivot->start ^ (curs+8)->start;
								result += pivot->start ^ (curs+9)->start;
								result += pivot->start ^ (curs+10)->start;
								result += pivot->start ^ (curs+11)->start;
								result += pivot->start ^ (curs+12)->start;
								result += pivot->start ^ (curs+13)->start;
								result += pivot->start ^ (curs+14)->start;
								result += pivot->start ^ (curs+15)->start;
								result += pivot->start ^ (curs+16)->start;
								result += pivot->start ^ (curs+17)->start;
								result += pivot->start ^ (curs+18)->start;
								result += pivot->start ^ (curs+19)->start;
								result += pivot->start ^ (curs+20)->start;
								result += pivot->start ^ (curs+21)->start;
								result += pivot->start ^ (curs+22)->start;
								result += pivot->start ^ (curs+23)->start;
								result += pivot->start ^ (curs+24)->start;
								result += pivot->start ^ (curs+25)->start;
								result += pivot->start ^ (curs+26)->start;
								result += pivot->start ^ (curs+27)->start;
								result += pivot->start ^ (curs+28)->start;
								result += pivot->start ^ (curs+29)->start;
								pivot++;
							}
							break;
							
						case 31:
							while (pivot < last)
							{
								result += pivot->start ^ (curs+0)->start;
								result += pivot->start ^ (curs+1)->start;
								result += pivot->start ^ (curs+2)->start;
								result += pivot->start ^ (curs+3)->start;
								result += pivot->start ^ (curs+4)->start;
								result += pivot->start ^ (curs+5)->start;
								result += pivot->start ^ (curs+6)->start;
								result += pivot->start ^ (curs+7)->start;
								result += pivot->start ^ (curs+8)->start;
								result += pivot->start ^ (curs+9)->start;
								result += pivot->start ^ (curs+10)->start;
								result += pivot->start ^ (curs+11)->start;
								result += pivot->start ^ (curs+12)->start;
								result += pivot->start ^ (curs+13)->start;
								result += pivot->start ^ (curs+14)->start;
								result += pivot->start ^ (curs+15)->start;
								result += pivot->start ^ (curs+16)->start;
								result += pivot->start ^ (curs+17)->start;
								result += pivot->start ^ (curs+18)->start;
								result += pivot->start ^ (curs+19)->start;
								result += pivot->start ^ (curs+20)->start;
								result += pivot->start ^ (curs+21)->start;
								result += pivot->start ^ (curs+22)->start;
								result += pivot->start ^ (curs+23)->start;
								result += pivot->start ^ (curs+24)->start;
								result += pivot->start ^ (curs+25)->start;
								result += pivot->start ^ (curs+26)->start;
								result += pivot->start ^ (curs+27)->start;
								result += pivot->start ^ (curs+28)->start;
								result += pivot->start ^ (curs+29)->start;
								result += pivot->start ^ (curs+30)->start;
								pivot++;
							}
							break;
							
						case 32:
							while (pivot < last)
							{
								result += pivot->start ^ (curs+0)->start;
								result += pivot->start ^ (curs+1)->start;
								result += pivot->start ^ (curs+2)->start;
								result += pivot->start ^ (curs+3)->start;
								result += pivot->start ^ (curs+4)->start;
								result += pivot->start ^ (curs+5)->start;
								result += pivot->start ^ (curs+6)->start;
								result += pivot->start ^ (curs+7)->start;
								result += pivot->start ^ (curs+8)->start;
								result += pivot->start ^ (curs+9)->start;
								result += pivot->start ^ (curs+10)->start;
								result += pivot->start ^ (curs+11)->start;
								result += pivot->start ^ (curs+12)->start;
								result += pivot->start ^ (curs+13)->start;
								result += pivot->start ^ (curs+14)->start;
								result += pivot->start ^ (curs+15)->start;
								result += pivot->start ^ (curs+16)->start;
								result += pivot->start ^ (curs+17)->start;
								result += pivot->start ^ (curs+18)->start;
								result += pivot->start ^ (curs+19)->start;
								result += pivot->start ^ (curs+20)->start;
								result += pivot->start ^ (curs+21)->start;
								result += pivot->start ^ (curs+22)->start;
								result += pivot->start ^ (curs+23)->start;
								result += pivot->start ^ (curs+24)->start;
								result += pivot->start ^ (curs+25)->start;
								result += pivot->start ^ (curs+26)->start;
								result += pivot->start ^ (curs+27)->start;
								result += pivot->start ^ (curs+28)->start;
								result += pivot->start ^ (curs+29)->start;
								result += pivot->start ^ (curs+30)->start;
								result += pivot->start ^ (curs+31)->start;
								pivot++;
							}
							break;
							
						default:
							while (last-pivot >= 32)
							{
								for (auto k = curs; k != lastGS; k++)
								{
									result += (pivot+0)->start ^ k->start;
									result += (pivot+1)->start ^ k->start;
									result += (pivot+2)->start ^ k->start;
									result += (pivot+3)->start ^ k->start;
									result += (pivot+4)->start ^ k->start;
									result += (pivot+5)->start ^ k->start;
									result += (pivot+6)->start ^ k->start;
									result += (pivot+7)->start ^ k->start;
									result += (pivot+8)->start ^ k->start;
									result += (pivot+9)->start ^ k->start;
									result += (pivot+10)->start ^ k->start;
									result += (pivot+11)->start ^ k->start;
									result += (pivot+12)->start ^ k->start;
									result += (pivot+13)->start ^ k->start;
									result += (pivot+14)->start ^ k->start;
									result += (pivot+15)->start ^ k->start;
									result += (pivot+16)->start ^ k->start;
									result += (pivot+17)->start ^ k->start;
									result += (pivot+18)->start ^ k->start;
									result += (pivot+19)->start ^ k->start;
									result += (pivot+20)->start ^ k->start;
									result += (pivot+21)->start ^ k->start;
									result += (pivot+22)->start ^ k->start;
									result += (pivot+23)->start ^ k->start;
									result += (pivot+24)->start ^ k->start;
									result += (pivot+25)->start ^ k->start;
									result += (pivot+26)->start ^ k->start;
									result += (pivot+27)->start ^ k->start;
									result += (pivot+28)->start ^ k->start;
									result += (pivot+29)->start ^ k->start;
									result += (pivot+30)->start ^ k->start;
									result += (pivot+31)->start ^ k->start;
								}
								pivot += 32;
							}
							
							if (last-pivot >= 16)
							{
								for (auto k = curs; k != lastGS; k++)
								{
									result += (pivot+0)->start ^ k->start;
									result += (pivot+1)->start ^ k->start;
									result += (pivot+2)->start ^ k->start;
									result += (pivot+3)->start ^ k->start;
									result += (pivot+4)->start ^ k->start;
									result += (pivot+5)->start ^ k->start;
									result += (pivot+6)->start ^ k->start;
									result += (pivot+7)->start ^ k->start;
									result += (pivot+8)->start ^ k->start;
									result += (pivot+9)->start ^ k->start;
									result += (pivot+10)->start ^ k->start;
									result += (pivot+11)->start ^ k->start;
									result += (pivot+12)->start ^ k->start;
									result += (pivot+13)->start ^ k->start;
									result += (pivot+14)->start ^ k->start;
									result += (pivot+15)->start ^ k->start;
								}
								pivot += 16;
							}
							
							if (last-pivot >= 8)
							{
								for (auto k = curs; k != lastGS; k++)
								{
									result += (pivot+0)->start ^ k->start;
									result += (pivot+1)->start ^ k->start;
									result += (pivot+2)->start ^ k->start;
									result += (pivot+3)->start ^ k->start;
									result += (pivot+4)->start ^ k->start;
									result += (pivot+5)->start ^ k->start;
									result += (pivot+6)->start ^ k->start;
									result += (pivot+7)->start ^ k->start;
								}
								pivot += 8;
							}
							
							if (last-pivot >= 4)
							{
								for (auto k = curs; k != lastGS; k++)
								{
									result += (pivot+0)->start ^ k->start;
									result += (pivot+1)->start ^ k->start;
									result += (pivot+2)->start ^ k->start;
									result += (pivot+3)->start ^ k->start;
								}
								pivot += 4;
							}
							
							while (pivot < last)
							{
								auto k = curs;
								while (lastGS-k >= 32)
								{
									result += pivot->start ^ (k+0)->start;
									result += pivot->start ^ (k+1)->start;
									result += pivot->start ^ (k+2)->start;
									result += pivot->start ^ (k+3)->start;
									result += pivot->start ^ (k+4)->start;
									result += pivot->start ^ (k+5)->start;
									result += pivot->start ^ (k+6)->start;
									result += pivot->start ^ (k+7)->start;
									result += pivot->start ^ (k+8)->start;
									result += pivot->start ^ (k+9)->start;
									result += pivot->start ^ (k+10)->start;
									result += pivot->start ^ (k+11)->start;
									result += pivot->start ^ (k+12)->start;
									result += pivot->start ^ (k+13)->start;
									result += pivot->start ^ (k+14)->start;
									result += pivot->start ^ (k+15)->start;
									result += pivot->start ^ (k+16)->start;
									result += pivot->start ^ (k+17)->start;
									result += pivot->start ^ (k+18)->start;
									result += pivot->start ^ (k+19)->start;
									result += pivot->start ^ (k+20)->start;
									result += pivot->start ^ (k+21)->start;
									result += pivot->start ^ (k+22)->start;
									result += pivot->start ^ (k+23)->start;
									result += pivot->start ^ (k+24)->start;
									result += pivot->start ^ (k+25)->start;
									result += pivot->start ^ (k+26)->start;
									result += pivot->start ^ (k+27)->start;
									result += pivot->start ^ (k+28)->start;
									result += pivot->start ^ (k+29)->start;
									result += pivot->start ^ (k+30)->start;
									result += pivot->start ^ (k+31)->start;
									k += 32;
								}
								
								if (lastGS-k >= 16)
								{
									result += pivot->start ^ (k+0)->start;
									result += pivot->start ^ (k+1)->start;
									result += pivot->start ^ (k+2)->start;
									result += pivot->start ^ (k+3)->start;
									result += pivot->start ^ (k+4)->start;
									result += pivot->start ^ (k+5)->start;
									result += pivot->start ^ (k+6)->start;
									result += pivot->start ^ (k+7)->start;
									result += pivot->start ^ (k+8)->start;
									result += pivot->start ^ (k+9)->start;
									result += pivot->start ^ (k+10)->start;
									result += pivot->start ^ (k+11)->start;
									result += pivot->start ^ (k+12)->start;
									result += pivot->start ^ (k+13)->start;
									result += pivot->start ^ (k+14)->start;
									result += pivot->start ^ (k+15)->start;
									k += 16;
								}
								
								if (lastGS-k >= 8)
								{
									result += pivot->start ^ (k+0)->start;
									result += pivot->start ^ (k+1)->start;
									result += pivot->start ^ (k+2)->start;
									result += pivot->start ^ (k+3)->start;
									result += pivot->start ^ (k+4)->start;
									result += pivot->start ^ (k+5)->start;
									result += pivot->start ^ (k+6)->start;
									result += pivot->start ^ (k+7)->start;
									k += 8;
								}
								
								if (lastGS-k >= 4)
								{
									result += pivot->start ^ (k+0)->start;
									result += pivot->start ^ (k+1)->start;
									result += pivot->start ^ (k+2)->start;
									result += pivot->start ^ (k+3)->start;
									k += 4;
								}
								
								while (k != lastGS)
								{
									result += pivot->start ^ k->start; k++;
								}
								
								pivot++;
							}
							break;
					}
				}
				
				// Sweep the last bucket.
				auto last = BIR[cbucket_id].last;
				switch (bufferSize)
				{
					case 1:
						while ((last-pivot >= 32) && (curs->end >= (pivot+31)->start))
						{
							result += (pivot+0)->start ^ curs->start;
							result += (pivot+1)->start ^ curs->start;
							result += (pivot+2)->start ^ curs->start;
							result += (pivot+3)->start ^ curs->start;
							result += (pivot+4)->start ^ curs->start;
							result += (pivot+5)->start ^ curs->start;
							result += (pivot+6)->start ^ curs->start;
							result += (pivot+7)->start ^ curs->start;
							result += (pivot+8)->start ^ curs->start;
							result += (pivot+9)->start ^ curs->start;
							result += (pivot+10)->start ^ curs->start;
							result += (pivot+11)->start ^ curs->start;
							result += (pivot+12)->start ^ curs->start;
							result += (pivot+13)->start ^ curs->start;
							result += (pivot+14)->start ^ curs->start;
							result += (pivot+15)->start ^ curs->start;
							result += (pivot+16)->start ^ curs->start;
							result += (pivot+17)->start ^ curs->start;
							result += (pivot+18)->start ^ curs->start;
							result += (pivot+19)->start ^ curs->start;
							result += (pivot+20)->start ^ curs->start;
							result += (pivot+21)->start ^ curs->start;
							result += (pivot+22)->start ^ curs->start;
							result += (pivot+23)->start ^ curs->start;
							result += (pivot+24)->start ^ curs->start;
							result += (pivot+25)->start ^ curs->start;
							result += (pivot+26)->start ^ curs->start;
							result += (pivot+27)->start ^ curs->start;
							result += (pivot+28)->start ^ curs->start;
							result += (pivot+29)->start ^ curs->start;
							result += (pivot+30)->start ^ curs->start;
							result += (pivot+31)->start ^ curs->start;
							pivot += 32;
						}
						
						while ((pivot < last) && (pivot->start <= curs->end))
						{
							result += pivot->start ^ curs->start;
							pivot++;
						}
						break;
						
					case 2:
						while ((last-pivot >= 16) && (curs->end >= (pivot+15)->start))
						{
							result += (pivot+0)->start ^ (curs+0)->start;
							result += (pivot+0)->start ^ (curs+1)->start;
							result += (pivot+1)->start ^ (curs+0)->start;
							result += (pivot+1)->start ^ (curs+1)->start;
							result += (pivot+2)->start ^ (curs+0)->start;
							result += (pivot+2)->start ^ (curs+1)->start;
							result += (pivot+3)->start ^ (curs+0)->start;
							result += (pivot+3)->start ^ (curs+1)->start;
							result += (pivot+4)->start ^ (curs+0)->start;
							result += (pivot+4)->start ^ (curs+1)->start;
							result += (pivot+5)->start ^ (curs+0)->start;
							result += (pivot+5)->start ^ (curs+1)->start;
							result += (pivot+6)->start ^ (curs+0)->start;
							result += (pivot+6)->start ^ (curs+1)->start;
							result += (pivot+7)->start ^ (curs+0)->start;
							result += (pivot+7)->start ^ (curs+1)->start;
							result += (pivot+8)->start ^ (curs+0)->start;
							result += (pivot+8)->start ^ (curs+1)->start;
							result += (pivot+9)->start ^ (curs+0)->start;
							result += (pivot+9)->start ^ (curs+1)->start;
							result += (pivot+10)->start ^ (curs+0)->start;
							result += (pivot+10)->start ^ (curs+1)->start;
							result += (pivot+11)->start ^ (curs+0)->start;
							result += (pivot+11)->start ^ (curs+1)->start;
							result += (pivot+12)->start ^ (curs+0)->start;
							result += (pivot+12)->start ^ (curs+1)->start;
							result += (pivot+13)->start ^ (curs+0)->start;
							result += (pivot+13)->start ^ (curs+1)->start;
							result += (pivot+14)->start ^ (curs+0)->start;
							result += (pivot+14)->start ^ (curs+1)->start;
							result += (pivot+15)->start ^ (curs+0)->start;
							result += (pivot+15)->start ^ (curs+1)->start;
							pivot += 16;
						}
						
						while ((pivot < last) && (pivot->start <= curs->end))
						{
							result += pivot->start ^ (curs+0)->start;
							result += pivot->start ^ (curs+1)->start;
							pivot++;
						}
						
						break;
						
					case 3:
						while ((last-pivot >= 8) && (curs->end >= (pivot+7)->start))
						{
							result += (pivot+0)->start ^ (curs+0)->start;
							result += (pivot+0)->start ^ (curs+1)->start;
							result += (pivot+0)->start ^ (curs+2)->start;
							result += (pivot+1)->start ^ (curs+0)->start;
							result += (pivot+1)->start ^ (curs+1)->start;
							result += (pivot+1)->start ^ (curs+2)->start;
							result += (pivot+2)->start ^ (curs+0)->start;
							result += (pivot+2)->start ^ (curs+1)->start;
							result += (pivot+2)->start ^ (curs+2)->start;
							result += (pivot+3)->start ^ (curs+0)->start;
							result += (pivot+3)->start ^ (curs+1)->start;
							result += (pivot+3)->start ^ (curs+2)->start;
							result += (pivot+4)->start ^ (curs+0)->start;
							result += (pivot+4)->start ^ (curs+1)->start;
							result += (pivot+4)->start ^ (curs+2)->start;
							result += (pivot+5)->start ^ (curs+0)->start;
							result += (pivot+5)->start ^ (curs+1)->start;
							result += (pivot+5)->start ^ (curs+2)->start;
							result += (pivot+6)->start ^ (curs+0)->start;
							result += (pivot+6)->start ^ (curs+1)->start;
							result += (pivot+6)->start ^ (curs+2)->start;
							result += (pivot+7)->start ^ (curs+0)->start;
							result += (pivot+7)->start ^ (curs+1)->start;
							result += (pivot+7)->start ^ (curs+2)->start;
							pivot += 8;
						}
						
						while ((pivot < last) && (pivot->start <= curs->end))
						{
							result += pivot->start ^ (curs+0)->start;
							result += pivot->start ^ (curs+1)->start;
							result += pivot->start ^ (curs+2)->start;
							pivot++;
						}
						break;
						
					case 4:
						while ((last-pivot >= 8) && (curs->end >= (pivot+7)->start))
						{
							result += (pivot+0)->start ^ (curs+0)->start;
							result += (pivot+0)->start ^ (curs+1)->start;
							result += (pivot+0)->start ^ (curs+2)->start;
							result += (pivot+0)->start ^ (curs+3)->start;
							result += (pivot+1)->start ^ (curs+0)->start;
							result += (pivot+1)->start ^ (curs+1)->start;
							result += (pivot+1)->start ^ (curs+2)->start;
							result += (pivot+1)->start ^ (curs+3)->start;
							result += (pivot+2)->start ^ (curs+0)->start;
							result += (pivot+2)->start ^ (curs+1)->start;
							result += (pivot+2)->start ^ (curs+2)->start;
							result += (pivot+2)->start ^ (curs+3)->start;
							result += (pivot+3)->start ^ (curs+0)->start;
							result += (pivot+3)->start ^ (curs+1)->start;
							result += (pivot+3)->start ^ (curs+2)->start;
							result += (pivot+3)->start ^ (curs+3)->start;
							result += (pivot+4)->start ^ (curs+0)->start;
							result += (pivot+4)->start ^ (curs+1)->start;
							result += (pivot+4)->start ^ (curs+2)->start;
							result += (pivot+4)->start ^ (curs+3)->start;
							result += (pivot+5)->start ^ (curs+0)->start;
							result += (pivot+5)->start ^ (curs+1)->start;
							result += (pivot+5)->start ^ (curs+2)->start;
							result += (pivot+5)->start ^ (curs+3)->start;
							result += (pivot+6)->start ^ (curs+0)->start;
							result += (pivot+6)->start ^ (curs+1)->start;
							result += (pivot+6)->start ^ (curs+2)->start;
							result += (pivot+6)->start ^ (curs+3)->start;
							result += (pivot+7)->start ^ (curs+0)->start;
							result += (pivot+7)->start ^ (curs+1)->start;
							result += (pivot+7)->start ^ (curs+2)->start;
							result += (pivot+7)->start ^ (curs+3)->start;
							pivot += 8;
						}
						
						while ((pivot < last) && (pivot->start <= curs->end))
						{
							result += pivot->start ^ (curs+0)->start;
							result += pivot->start ^ (curs+1)->start;
							result += pivot->start ^ (curs+2)->start;
							result += pivot->start ^ (curs+3)->start;
							pivot++;
						}
						break;
						
					case 5:
						while ((pivot < last) && (pivot->start <= curs->end))
						{
							result += pivot->start ^ (curs+0)->start;
							result += pivot->start ^ (curs+1)->start;
							result += pivot->start ^ (curs+2)->start;
							result += pivot->start ^ (curs+3)->start;
							result += pivot->start ^ (curs+4)->start;
							pivot++;
						}
						break;
						
					case 6:
						while ((pivot < last) && (pivot->start <= curs->end))
						{
							result += pivot->start ^ (curs+0)->start;
							result += pivot->start ^ (curs+1)->start;
							result += pivot->start ^ (curs+2)->start;
							result += pivot->start ^ (curs+3)->start;
							result += pivot->start ^ (curs+4)->start;
							result += pivot->start ^ (curs+5)->start;
							pivot++;
						}
						break;
						
					case 7:
						while ((pivot < last) && (pivot->start <= curs->end))
						{
							result += pivot->start ^ (curs+0)->start;
							result += pivot->start ^ (curs+1)->start;
							result += pivot->start ^ (curs+2)->start;
							result += pivot->start ^ (curs+3)->start;
							result += pivot->start ^ (curs+4)->start;
							result += pivot->start ^ (curs+5)->start;
							result += pivot->start ^ (curs+6)->start;
							pivot++;
						}
						break;
						
					case 8:
						while ((pivot < last) && (pivot->start <= curs->end))
						{
							result += pivot->start ^ (curs+0)->start;
							result += pivot->start ^ (curs+1)->start;
							result += pivot->start ^ (curs+2)->start;
							result += pivot->start ^ (curs+3)->start;
							result += pivot->start ^ (curs+4)->start;
							result += pivot->start ^ (curs+5)->start;
							result += pivot->start ^ (curs+6)->start;
							result += pivot->start ^ (curs+7)->start;
							pivot++;
						}
						break;
						
					case 9:
						while ((pivot < last) && (pivot->start <= curs->end))
						{
							result += pivot->start ^ (curs+0)->start;
							result += pivot->start ^ (curs+1)->start;
							result += pivot->start ^ (curs+2)->start;
							result += pivot->start ^ (curs+3)->start;
							result += pivot->start ^ (curs+4)->start;
							result += pivot->start ^ (curs+5)->start;
							result += pivot->start ^ (curs+6)->start;
							result += pivot->start ^ (curs+7)->start;
							result += pivot->start ^ (curs+8)->start;
							pivot++;
						}
						break;
						
					case 10:
						while ((pivot < last) && (pivot->start <= curs->end))
						{
							result += pivot->start ^ (curs+0)->start;
							result += pivot->start ^ (curs+1)->start;
							result += pivot->start ^ (curs+2)->start;
							result += pivot->start ^ (curs+3)->start;
							result += pivot->start ^ (curs+4)->start;
							result += pivot->start ^ (curs+5)->start;
							result += pivot->start ^ (curs+6)->start;
							result += pivot->start ^ (curs+7)->start;
							result += pivot->start ^ (curs+8)->start;
							result += pivot->start ^ (curs+9)->start;
							pivot++;
						}
						break;
						
					case 11:
						while ((pivot < last) && (pivot->start <= curs->end))
						{
							result += pivot->start ^ (curs+0)->start;
							result += pivot->start ^ (curs+1)->start;
							result += pivot->start ^ (curs+2)->start;
							result += pivot->start ^ (curs+3)->start;
							result += pivot->start ^ (curs+4)->start;
							result += pivot->start ^ (curs+5)->start;
							result += pivot->start ^ (curs+6)->start;
							result += pivot->start ^ (curs+7)->start;
							result += pivot->start ^ (curs+8)->start;
							result += pivot->start ^ (curs+9)->start;
							result += pivot->start ^ (curs+10)->start;
							pivot++;
						}
						break;
						
					case 12:
						while ((pivot < last) && (pivot->start <= curs->end))
						{
							result += pivot->start ^ (curs+0)->start;
							result += pivot->start ^ (curs+1)->start;
							result += pivot->start ^ (curs+2)->start;
							result += pivot->start ^ (curs+3)->start;
							result += pivot->start ^ (curs+4)->start;
							result += pivot->start ^ (curs+5)->start;
							result += pivot->start ^ (curs+6)->start;
							result += pivot->start ^ (curs+7)->start;
							result += pivot->start ^ (curs+8)->start;
							result += pivot->start ^ (curs+9)->start;
							result += pivot->start ^ (curs+10)->start;
							result += pivot->start ^ (curs+11)->start;
							pivot++;
						}
						break;
						
					case 13:
						while ((pivot < last) && (pivot->start <= curs->end))
						{
							result += pivot->start ^ (curs+0)->start;
							result += pivot->start ^ (curs+1)->start;
							result += pivot->start ^ (curs+2)->start;
							result += pivot->start ^ (curs+3)->start;
							result += pivot->start ^ (curs+4)->start;
							result += pivot->start ^ (curs+5)->start;
							result += pivot->start ^ (curs+6)->start;
							result += pivot->start ^ (curs+7)->start;
							result += pivot->start ^ (curs+8)->start;
							result += pivot->start ^ (curs+9)->start;
							result += pivot->start ^ (curs+10)->start;
							result += pivot->start ^ (curs+11)->start;
							result += pivot->start ^ (curs+12)->start;
							pivot++;
						}
						break;
						
					case 14:
						while ((pivot < last) && (pivot->start <= curs->end))
						{
							result += pivot->start ^ (curs+0)->start;
							result += pivot->start ^ (curs+1)->start;
							result += pivot->start ^ (curs+2)->start;
							result += pivot->start ^ (curs+3)->start;
							result += pivot->start ^ (curs+4)->start;
							result += pivot->start ^ (curs+5)->start;
							result += pivot->start ^ (curs+6)->start;
							result += pivot->start ^ (curs+7)->start;
							result += pivot->start ^ (curs+8)->start;
							result += pivot->start ^ (curs+9)->start;
							result += pivot->start ^ (curs+10)->start;
							result += pivot->start ^ (curs+11)->start;
							result += pivot->start ^ (curs+12)->start;
							result += pivot->start ^ (curs+13)->start;
							pivot++;
						}
						break;
						
					case 15:
						while ((pivot < last) && (pivot->start <= curs->end))
						{
							result += pivot->start ^ (curs+0)->start;
							result += pivot->start ^ (curs+1)->start;
							result += pivot->start ^ (curs+2)->start;
							result += pivot->start ^ (curs+3)->start;
							result += pivot->start ^ (curs+4)->start;
							result += pivot->start ^ (curs+5)->start;
							result += pivot->start ^ (curs+6)->start;
							result += pivot->start ^ (curs+7)->start;
							result += pivot->start ^ (curs+8)->start;
							result += pivot->start ^ (curs+9)->start;
							result += pivot->start ^ (curs+10)->start;
							result += pivot->start ^ (curs+11)->start;
							result += pivot->start ^ (curs+12)->start;
							result += pivot->start ^ (curs+13)->start;
							result += pivot->start ^ (curs+14)->start;
							pivot++;
						}
						break;
						
					case 16:
						while ((pivot < last) && (pivot->start <= curs->end))
						{
							result += pivot->start ^ (curs+0)->start;
							result += pivot->start ^ (curs+1)->start;
							result += pivot->start ^ (curs+2)->start;
							result += pivot->start ^ (curs+3)->start;
							result += pivot->start ^ (curs+4)->start;
							result += pivot->start ^ (curs+5)->start;
							result += pivot->start ^ (curs+6)->start;
							result += pivot->start ^ (curs+7)->start;
							result += pivot->start ^ (curs+8)->start;
							result += pivot->start ^ (curs+9)->start;
							result += pivot->start ^ (curs+10)->start;
							result += pivot->start ^ (curs+11)->start;
							result += pivot->start ^ (curs+12)->start;
							result += pivot->start ^ (curs+13)->start;
							result += pivot->start ^ (curs+14)->start;
							result += pivot->start ^ (curs+15)->start;
							pivot++;
						}
						break;
						
					case 17:
						while ((pivot < last) && (pivot->start <= curs->end))
						{
							result += pivot->start ^ (curs+0)->start;
							result += pivot->start ^ (curs+1)->start;
							result += pivot->start ^ (curs+2)->start;
							result += pivot->start ^ (curs+3)->start;
							result += pivot->start ^ (curs+4)->start;
							result += pivot->start ^ (curs+5)->start;
							result += pivot->start ^ (curs+6)->start;
							result += pivot->start ^ (curs+7)->start;
							result += pivot->start ^ (curs+8)->start;
							result += pivot->start ^ (curs+9)->start;
							result += pivot->start ^ (curs+10)->start;
							result += pivot->start ^ (curs+11)->start;
							result += pivot->start ^ (curs+12)->start;
							result += pivot->start ^ (curs+13)->start;
							result += pivot->start ^ (curs+14)->start;
							result += pivot->start ^ (curs+15)->start;
							result += pivot->start ^ (curs+16)->start;
							pivot++;
						}
						break;
						
					case 18:
						while ((pivot < last) && (pivot->start <= curs->end))
						{
							result += pivot->start ^ (curs+0)->start;
							result += pivot->start ^ (curs+1)->start;
							result += pivot->start ^ (curs+2)->start;
							result += pivot->start ^ (curs+3)->start;
							result += pivot->start ^ (curs+4)->start;
							result += pivot->start ^ (curs+5)->start;
							result += pivot->start ^ (curs+6)->start;
							result += pivot->start ^ (curs+7)->start;
							result += pivot->start ^ (curs+8)->start;
							result += pivot->start ^ (curs+9)->start;
							result += pivot->start ^ (curs+10)->start;
							result += pivot->start ^ (curs+11)->start;
							result += pivot->start ^ (curs+12)->start;
							result += pivot->start ^ (curs+13)->start;
							result += pivot->start ^ (curs+14)->start;
							result += pivot->start ^ (curs+15)->start;
							result += pivot->start ^ (curs+16)->start;
							result += pivot->start ^ (curs+17)->start;
							pivot++;
						}
						break;
						
					case 19:
						while ((pivot < last) && (pivot->start <= curs->end))
						{
							result += pivot->start ^ (curs+0)->start;
							result += pivot->start ^ (curs+1)->start;
							result += pivot->start ^ (curs+2)->start;
							result += pivot->start ^ (curs+3)->start;
							result += pivot->start ^ (curs+4)->start;
							result += pivot->start ^ (curs+5)->start;
							result += pivot->start ^ (curs+6)->start;
							result += pivot->start ^ (curs+7)->start;
							result += pivot->start ^ (curs+8)->start;
							result += pivot->start ^ (curs+9)->start;
							result += pivot->start ^ (curs+10)->start;
							result += pivot->start ^ (curs+11)->start;
							result += pivot->start ^ (curs+12)->start;
							result += pivot->start ^ (curs+13)->start;
							result += pivot->start ^ (curs+14)->start;
							result += pivot->start ^ (curs+15)->start;
							result += pivot->start ^ (curs+16)->start;
							result += pivot->start ^ (curs+17)->start;
							result += pivot->start ^ (curs+18)->start;
							pivot++;
						}
						break;
						
					case 20:
						while ((pivot < last) && (pivot->start <= curs->end))
						{
							result += pivot->start ^ (curs+0)->start;
							result += pivot->start ^ (curs+1)->start;
							result += pivot->start ^ (curs+2)->start;
							result += pivot->start ^ (curs+3)->start;
							result += pivot->start ^ (curs+4)->start;
							result += pivot->start ^ (curs+5)->start;
							result += pivot->start ^ (curs+6)->start;
							result += pivot->start ^ (curs+7)->start;
							result += pivot->start ^ (curs+8)->start;
							result += pivot->start ^ (curs+9)->start;
							result += pivot->start ^ (curs+10)->start;
							result += pivot->start ^ (curs+11)->start;
							result += pivot->start ^ (curs+12)->start;
							result += pivot->start ^ (curs+13)->start;
							result += pivot->start ^ (curs+14)->start;
							result += pivot->start ^ (curs+15)->start;
							result += pivot->start ^ (curs+16)->start;
							result += pivot->start ^ (curs+17)->start;
							result += pivot->start ^ (curs+18)->start;
							result += pivot->start ^ (curs+19)->start;
							pivot++;
						}
						break;
						
					case 21:
						while ((pivot < last) && (pivot->start <= curs->end))
						{
							result += pivot->start ^ (curs+0)->start;
							result += pivot->start ^ (curs+1)->start;
							result += pivot->start ^ (curs+2)->start;
							result += pivot->start ^ (curs+3)->start;
							result += pivot->start ^ (curs+4)->start;
							result += pivot->start ^ (curs+5)->start;
							result += pivot->start ^ (curs+6)->start;
							result += pivot->start ^ (curs+7)->start;
							result += pivot->start ^ (curs+8)->start;
							result += pivot->start ^ (curs+9)->start;
							result += pivot->start ^ (curs+10)->start;
							result += pivot->start ^ (curs+11)->start;
							result += pivot->start ^ (curs+12)->start;
							result += pivot->start ^ (curs+13)->start;
							result += pivot->start ^ (curs+14)->start;
							result += pivot->start ^ (curs+15)->start;
							result += pivot->start ^ (curs+16)->start;
							result += pivot->start ^ (curs+17)->start;
							result += pivot->start ^ (curs+18)->start;
							result += pivot->start ^ (curs+19)->start;
							result += pivot->start ^ (curs+20)->start;
							pivot++;
						}
						break;
						
					case 22:
						while ((pivot < last) && (pivot->start <= curs->end))
						{
							result += pivot->start ^ (curs+0)->start;
							result += pivot->start ^ (curs+1)->start;
							result += pivot->start ^ (curs+2)->start;
							result += pivot->start ^ (curs+3)->start;
							result += pivot->start ^ (curs+4)->start;
							result += pivot->start ^ (curs+5)->start;
							result += pivot->start ^ (curs+6)->start;
							result += pivot->start ^ (curs+7)->start;
							result += pivot->start ^ (curs+8)->start;
							result += pivot->start ^ (curs+9)->start;
							result += pivot->start ^ (curs+10)->start;
							result += pivot->start ^ (curs+11)->start;
							result += pivot->start ^ (curs+12)->start;
							result += pivot->start ^ (curs+13)->start;
							result += pivot->start ^ (curs+14)->start;
							result += pivot->start ^ (curs+15)->start;
							result += pivot->start ^ (curs+16)->start;
							result += pivot->start ^ (curs+17)->start;
							result += pivot->start ^ (curs+18)->start;
							result += pivot->start ^ (curs+19)->start;
							result += pivot->start ^ (curs+20)->start;
							result += pivot->start ^ (curs+21)->start;
							pivot++;
						}
						break;
						
					case 23:
						while ((pivot < last) && (pivot->start <= curs->end))
						{
							result += pivot->start ^ (curs+0)->start;
							result += pivot->start ^ (curs+1)->start;
							result += pivot->start ^ (curs+2)->start;
							result += pivot->start ^ (curs+3)->start;
							result += pivot->start ^ (curs+4)->start;
							result += pivot->start ^ (curs+5)->start;
							result += pivot->start ^ (curs+6)->start;
							result += pivot->start ^ (curs+7)->start;
							result += pivot->start ^ (curs+8)->start;
							result += pivot->start ^ (curs+9)->start;
							result += pivot->start ^ (curs+10)->start;
							result += pivot->start ^ (curs+11)->start;
							result += pivot->start ^ (curs+12)->start;
							result += pivot->start ^ (curs+13)->start;
							result += pivot->start ^ (curs+14)->start;
							result += pivot->start ^ (curs+15)->start;
							result += pivot->start ^ (curs+16)->start;
							result += pivot->start ^ (curs+17)->start;
							result += pivot->start ^ (curs+18)->start;
							result += pivot->start ^ (curs+19)->start;
							result += pivot->start ^ (curs+20)->start;
							result += pivot->start ^ (curs+21)->start;
							result += pivot->start ^ (curs+22)->start;
							pivot++;
						}
						break;
						
					case 24:
						while ((pivot < last) && (pivot->start <= curs->end))
						{
							result += pivot->start ^ (curs+0)->start;
							result += pivot->start ^ (curs+1)->start;
							result += pivot->start ^ (curs+2)->start;
							result += pivot->start ^ (curs+3)->start;
							result += pivot->start ^ (curs+4)->start;
							result += pivot->start ^ (curs+5)->start;
							result += pivot->start ^ (curs+6)->start;
							result += pivot->start ^ (curs+7)->start;
							result += pivot->start ^ (curs+8)->start;
							result += pivot->start ^ (curs+9)->start;
							result += pivot->start ^ (curs+10)->start;
							result += pivot->start ^ (curs+11)->start;
							result += pivot->start ^ (curs+12)->start;
							result += pivot->start ^ (curs+13)->start;
							result += pivot->start ^ (curs+14)->start;
							result += pivot->start ^ (curs+15)->start;
							result += pivot->start ^ (curs+16)->start;
							result += pivot->start ^ (curs+17)->start;
							result += pivot->start ^ (curs+18)->start;
							result += pivot->start ^ (curs+19)->start;
							result += pivot->start ^ (curs+20)->start;
							result += pivot->start ^ (curs+21)->start;
							result += pivot->start ^ (curs+22)->start;
							result += pivot->start ^ (curs+23)->start;
							pivot++;
						}
						break;
						
					case 25:
						while ((pivot < last) && (pivot->start <= curs->end))
						{
							result += pivot->start ^ (curs+0)->start;
							result += pivot->start ^ (curs+1)->start;
							result += pivot->start ^ (curs+2)->start;
							result += pivot->start ^ (curs+3)->start;
							result += pivot->start ^ (curs+4)->start;
							result += pivot->start ^ (curs+5)->start;
							result += pivot->start ^ (curs+6)->start;
							result += pivot->start ^ (curs+7)->start;
							result += pivot->start ^ (curs+8)->start;
							result += pivot->start ^ (curs+9)->start;
							result += pivot->start ^ (curs+10)->start;
							result += pivot->start ^ (curs+11)->start;
							result += pivot->start ^ (curs+12)->start;
							result += pivot->start ^ (curs+13)->start;
							result += pivot->start ^ (curs+14)->start;
							result += pivot->start ^ (curs+15)->start;
							result += pivot->start ^ (curs+16)->start;
							result += pivot->start ^ (curs+17)->start;
							result += pivot->start ^ (curs+18)->start;
							result += pivot->start ^ (curs+19)->start;
							result += pivot->start ^ (curs+20)->start;
							result += pivot->start ^ (curs+21)->start;
							result += pivot->start ^ (curs+22)->start;
							result += pivot->start ^ (curs+23)->start;
							result += pivot->start ^ (curs+24)->start;
							pivot++;
						}
						break;
						
					case 26:
						while ((pivot < last) && (pivot->start <= curs->end))
						{
							result += pivot->start ^ (curs+0)->start;
							result += pivot->start ^ (curs+1)->start;
							result += pivot->start ^ (curs+2)->start;
							result += pivot->start ^ (curs+3)->start;
							result += pivot->start ^ (curs+4)->start;
							result += pivot->start ^ (curs+5)->start;
							result += pivot->start ^ (curs+6)->start;
							result += pivot->start ^ (curs+7)->start;
							result += pivot->start ^ (curs+8)->start;
							result += pivot->start ^ (curs+9)->start;
							result += pivot->start ^ (curs+10)->start;
							result += pivot->start ^ (curs+11)->start;
							result += pivot->start ^ (curs+12)->start;
							result += pivot->start ^ (curs+13)->start;
							result += pivot->start ^ (curs+14)->start;
							result += pivot->start ^ (curs+15)->start;
							result += pivot->start ^ (curs+16)->start;
							result += pivot->start ^ (curs+17)->start;
							result += pivot->start ^ (curs+18)->start;
							result += pivot->start ^ (curs+19)->start;
							result += pivot->start ^ (curs+20)->start;
							result += pivot->start ^ (curs+21)->start;
							result += pivot->start ^ (curs+22)->start;
							result += pivot->start ^ (curs+23)->start;
							result += pivot->start ^ (curs+24)->start;
							result += pivot->start ^ (curs+25)->start;
							pivot++;
						}
						break;
						
					case 27:
						while ((pivot < last) && (pivot->start <= curs->end))
						{
							result += pivot->start ^ (curs+0)->start;
							result += pivot->start ^ (curs+1)->start;
							result += pivot->start ^ (curs+2)->start;
							result += pivot->start ^ (curs+3)->start;
							result += pivot->start ^ (curs+4)->start;
							result += pivot->start ^ (curs+5)->start;
							result += pivot->start ^ (curs+6)->start;
							result += pivot->start ^ (curs+7)->start;
							result += pivot->start ^ (curs+8)->start;
							result += pivot->start ^ (curs+9)->start;
							result += pivot->start ^ (curs+10)->start;
							result += pivot->start ^ (curs+11)->start;
							result += pivot->start ^ (curs+12)->start;
							result += pivot->start ^ (curs+13)->start;
							result += pivot->start ^ (curs+14)->start;
							result += pivot->start ^ (curs+15)->start;
							result += pivot->start ^ (curs+16)->start;
							result += pivot->start ^ (curs+17)->start;
							result += pivot->start ^ (curs+18)->start;
							result += pivot->start ^ (curs+19)->start;
							result += pivot->start ^ (curs+20)->start;
							result += pivot->start ^ (curs+21)->start;
							result += pivot->start ^ (curs+22)->start;
							result += pivot->start ^ (curs+23)->start;
							result += pivot->start ^ (curs+24)->start;
							result += pivot->start ^ (curs+25)->start;
							result += pivot->start ^ (curs+26)->start;
							pivot++;
						}
						break;
						
					case 28:
						while ((pivot < last) && (pivot->start <= curs->end))
						{
							result += pivot->start ^ (curs+0)->start;
							result += pivot->start ^ (curs+1)->start;
							result += pivot->start ^ (curs+2)->start;
							result += pivot->start ^ (curs+3)->start;
							result += pivot->start ^ (curs+4)->start;
							result += pivot->start ^ (curs+5)->start;
							result += pivot->start ^ (curs+6)->start;
							result += pivot->start ^ (curs+7)->start;
							result += pivot->start ^ (curs+8)->start;
							result += pivot->start ^ (curs+9)->start;
							result += pivot->start ^ (curs+10)->start;
							result += pivot->start ^ (curs+11)->start;
							result += pivot->start ^ (curs+12)->start;
							result += pivot->start ^ (curs+13)->start;
							result += pivot->start ^ (curs+14)->start;
							result += pivot->start ^ (curs+15)->start;
							result += pivot->start ^ (curs+16)->start;
							result += pivot->start ^ (curs+17)->start;
							result += pivot->start ^ (curs+18)->start;
							result += pivot->start ^ (curs+19)->start;
							result += pivot->start ^ (curs+20)->start;
							result += pivot->start ^ (curs+21)->start;
							result += pivot->start ^ (curs+22)->start;
							result += pivot->start ^ (curs+23)->start;
							result += pivot->start ^ (curs+24)->start;
							result += pivot->start ^ (curs+25)->start;
							result += pivot->start ^ (curs+26)->start;
							result += pivot->start ^ (curs+27)->start;
							pivot++;
						}
						break;
						
					case 29:
						while ((pivot < last) && (pivot->start <= curs->end))
						{
							result += pivot->start ^ (curs+0)->start;
							result += pivot->start ^ (curs+1)->start;
							result += pivot->start ^ (curs+2)->start;
							result += pivot->start ^ (curs+3)->start;
							result += pivot->start ^ (curs+4)->start;
							result += pivot->start ^ (curs+5)->start;
							result += pivot->start ^ (curs+6)->start;
							result += pivot->start ^ (curs+7)->start;
							result += pivot->start ^ (curs+8)->start;
							result += pivot->start ^ (curs+9)->start;
							result += pivot->start ^ (curs+10)->start;
							result += pivot->start ^ (curs+11)->start;
							result += pivot->start ^ (curs+12)->start;
							result += pivot->start ^ (curs+13)->start;
							result += pivot->start ^ (curs+14)->start;
							result += pivot->start ^ (curs+15)->start;
							result += pivot->start ^ (curs+16)->start;
							result += pivot->start ^ (curs+17)->start;
							result += pivot->start ^ (curs+18)->start;
							result += pivot->start ^ (curs+19)->start;
							result += pivot->start ^ (curs+20)->start;
							result += pivot->start ^ (curs+21)->start;
							result += pivot->start ^ (curs+22)->start;
							result += pivot->start ^ (curs+23)->start;
							result += pivot->start ^ (curs+24)->start;
							result += pivot->start ^ (curs+25)->start;
							result += pivot->start ^ (curs+26)->start;
							result += pivot->start ^ (curs+27)->start;
							result += pivot->start ^ (curs+28)->start;
							pivot++;
						}
						break;
						
					case 30:
						while ((pivot < last) && (pivot->start <= curs->end))
						{
							result += pivot->start ^ (curs+0)->start;
							result += pivot->start ^ (curs+1)->start;
							result += pivot->start ^ (curs+2)->start;
							result += pivot->start ^ (curs+3)->start;
							result += pivot->start ^ (curs+4)->start;
							result += pivot->start ^ (curs+5)->start;
							result += pivot->start ^ (curs+6)->start;
							result += pivot->start ^ (curs+7)->start;
							result += pivot->start ^ (curs+8)->start;
							result += pivot->start ^ (curs+9)->start;
							result += pivot->start ^ (curs+10)->start;
							result += pivot->start ^ (curs+11)->start;
							result += pivot->start ^ (curs+12)->start;
							result += pivot->start ^ (curs+13)->start;
							result += pivot->start ^ (curs+14)->start;
							result += pivot->start ^ (curs+15)->start;
							result += pivot->start ^ (curs+16)->start;
							result += pivot->start ^ (curs+17)->start;
							result += pivot->start ^ (curs+18)->start;
							result += pivot->start ^ (curs+19)->start;
							result += pivot->start ^ (curs+20)->start;
							result += pivot->start ^ (curs+21)->start;
							result += pivot->start ^ (curs+22)->start;
							result += pivot->start ^ (curs+23)->start;
							result += pivot->start ^ (curs+24)->start;
							result += pivot->start ^ (curs+25)->start;
							result += pivot->start ^ (curs+26)->start;
							result += pivot->start ^ (curs+27)->start;
							result += pivot->start ^ (curs+28)->start;
							result += pivot->start ^ (curs+29)->start;
							pivot++;
						}
						break;
						
					case 31:
						while ((pivot < last) && (pivot->start <= curs->end))
						{
							result += pivot->start ^ (curs+0)->start;
							result += pivot->start ^ (curs+1)->start;
							result += pivot->start ^ (curs+2)->start;
							result += pivot->start ^ (curs+3)->start;
							result += pivot->start ^ (curs+4)->start;
							result += pivot->start ^ (curs+5)->start;
							result += pivot->start ^ (curs+6)->start;
							result += pivot->start ^ (curs+7)->start;
							result += pivot->start ^ (curs+8)->start;
							result += pivot->start ^ (curs+9)->start;
							result += pivot->start ^ (curs+10)->start;
							result += pivot->start ^ (curs+11)->start;
							result += pivot->start ^ (curs+12)->start;
							result += pivot->start ^ (curs+13)->start;
							result += pivot->start ^ (curs+14)->start;
							result += pivot->start ^ (curs+15)->start;
							result += pivot->start ^ (curs+16)->start;
							result += pivot->start ^ (curs+17)->start;
							result += pivot->start ^ (curs+18)->start;
							result += pivot->start ^ (curs+19)->start;
							result += pivot->start ^ (curs+20)->start;
							result += pivot->start ^ (curs+21)->start;
							result += pivot->start ^ (curs+22)->start;
							result += pivot->start ^ (curs+23)->start;
							result += pivot->start ^ (curs+24)->start;
							result += pivot->start ^ (curs+25)->start;
							result += pivot->start ^ (curs+26)->start;
							result += pivot->start ^ (curs+27)->start;
							result += pivot->start ^ (curs+28)->start;
							result += pivot->start ^ (curs+29)->start;
							result += pivot->start ^ (curs+30)->start;
							pivot++;
						}
						break;
						
					case 32:
						while ((pivot < last) && (pivot->start <= curs->end))
						{
							result += pivot->start ^ (curs+0)->start;
							result += pivot->start ^ (curs+1)->start;
							result += pivot->start ^ (curs+2)->start;
							result += pivot->start ^ (curs+3)->start;
							result += pivot->start ^ (curs+4)->start;
							result += pivot->start ^ (curs+5)->start;
							result += pivot->start ^ (curs+6)->start;
							result += pivot->start ^ (curs+7)->start;
							result += pivot->start ^ (curs+8)->start;
							result += pivot->start ^ (curs+9)->start;
							result += pivot->start ^ (curs+10)->start;
							result += pivot->start ^ (curs+11)->start;
							result += pivot->start ^ (curs+12)->start;
							result += pivot->start ^ (curs+13)->start;
							result += pivot->start ^ (curs+14)->start;
							result += pivot->start ^ (curs+15)->start;
							result += pivot->start ^ (curs+16)->start;
							result += pivot->start ^ (curs+17)->start;
							result += pivot->start ^ (curs+18)->start;
							result += pivot->start ^ (curs+19)->start;
							result += pivot->start ^ (curs+20)->start;
							result += pivot->start ^ (curs+21)->start;
							result += pivot->start ^ (curs+22)->start;
							result += pivot->start ^ (curs+23)->start;
							result += pivot->start ^ (curs+24)->start;
							result += pivot->start ^ (curs+25)->start;
							result += pivot->start ^ (curs+26)->start;
							result += pivot->start ^ (curs+27)->start;
							result += pivot->start ^ (curs+28)->start;
							result += pivot->start ^ (curs+29)->start;
							result += pivot->start ^ (curs+30)->start;
							result += pivot->start ^ (curs+31)->start;
							pivot++;
						}
						break;
						
					default:
						while ((last-pivot >= 32) && (curs->end >= (pivot+31)->start))
						{
							for (auto k = curs; k != lastGS; k++)
							{
								result += (pivot+0)->start ^ k->start;
								result += (pivot+1)->start ^ k->start;
								result += (pivot+2)->start ^ k->start;
								result += (pivot+3)->start ^ k->start;
								result += (pivot+4)->start ^ k->start;
								result += (pivot+5)->start ^ k->start;
								result += (pivot+6)->start ^ k->start;
								result += (pivot+7)->start ^ k->start;
								result += (pivot+8)->start ^ k->start;
								result += (pivot+9)->start ^ k->start;
								result += (pivot+10)->start ^ k->start;
								result += (pivot+11)->start ^ k->start;
								result += (pivot+12)->start ^ k->start;
								result += (pivot+13)->start ^ k->start;
								result += (pivot+14)->start ^ k->start;
								result += (pivot+15)->start ^ k->start;
								result += (pivot+16)->start ^ k->start;
								result += (pivot+17)->start ^ k->start;
								result += (pivot+18)->start ^ k->start;
								result += (pivot+19)->start ^ k->start;
								result += (pivot+20)->start ^ k->start;
								result += (pivot+21)->start ^ k->start;
								result += (pivot+22)->start ^ k->start;
								result += (pivot+23)->start ^ k->start;
								result += (pivot+24)->start ^ k->start;
								result += (pivot+25)->start ^ k->start;
								result += (pivot+26)->start ^ k->start;
								result += (pivot+27)->start ^ k->start;
								result += (pivot+28)->start ^ k->start;
								result += (pivot+29)->start ^ k->start;
								result += (pivot+30)->start ^ k->start;
								result += (pivot+31)->start ^ k->start;
							}
							pivot += 32;
						}
						
						if ((last-pivot >= 16) && (curs->end >= (pivot+15)->start))
						{
							for (auto k = curs; k != lastGS; k++)
							{
								result += (pivot+0)->start ^ k->start;
								result += (pivot+1)->start ^ k->start;
								result += (pivot+2)->start ^ k->start;
								result += (pivot+3)->start ^ k->start;
								result += (pivot+4)->start ^ k->start;
								result += (pivot+5)->start ^ k->start;
								result += (pivot+6)->start ^ k->start;
								result += (pivot+7)->start ^ k->start;
								result += (pivot+8)->start ^ k->start;
								result += (pivot+9)->start ^ k->start;
								result += (pivot+10)->start ^ k->start;
								result += (pivot+11)->start ^ k->start;
								result += (pivot+12)->start ^ k->start;
								result += (pivot+13)->start ^ k->start;
								result += (pivot+14)->start ^ k->start;
								result += (pivot+15)->start ^ k->start;
							}
							pivot += 16;
						}
						
						if ((last-pivot >= 8) && (curs->end >= (pivot+7)->start))
						{
							for (auto k = curs; k != lastGS; k++)
							{
								result += (pivot+0)->start ^ k->start;
								result += (pivot+1)->start ^ k->start;
								result += (pivot+2)->start ^ k->start;
								result += (pivot+3)->start ^ k->start;
								result += (pivot+4)->start ^ k->start;
								result += (pivot+5)->start ^ k->start;
								result += (pivot+6)->start ^ k->start;
								result += (pivot+7)->start ^ k->start;
							}
							pivot += 8;
						}
						
						if ((last-pivot >= 4) && (curs->end >= (pivot+3)->start))
						{
							for (auto k = curs; k != lastGS; k++)
							{
								result += (pivot+0)->start ^ k->start;
								result += (pivot+1)->start ^ k->start;
								result += (pivot+2)->start ^ k->start;
								result += (pivot+3)->start ^ k->start;
							}
							pivot += 4;
						}
						
						while ((pivot < last) && (pivot->start <= curs->end))
						{
							auto k = curs;
							while (lastGS-k >= 32)
							{
								result += pivot->start ^ (k+0)->start;
								result += pivot->start ^ (k+1)->start;
								result += pivot->start ^ (k+2)->start;
								result += pivot->start ^ (k+3)->start;
								result += pivot->start ^ (k+4)->start;
								result += pivot->start ^ (k+5)->start;
								result += pivot->start ^ (k+6)->start;
								result += pivot->start ^ (k+7)->start;
								result += pivot->start ^ (k+8)->start;
								result += pivot->start ^ (k+9)->start;
								result += pivot->start ^ (k+10)->start;
								result += pivot->start ^ (k+11)->start;
								result += pivot->start ^ (k+12)->start;
								result += pivot->start ^ (k+13)->start;
								result += pivot->start ^ (k+14)->start;
								result += pivot->start ^ (k+15)->start;
								result += pivot->start ^ (k+16)->start;
								result += pivot->start ^ (k+17)->start;
								result += pivot->start ^ (k+18)->start;
								result += pivot->start ^ (k+19)->start;
								result += pivot->start ^ (k+20)->start;
								result += pivot->start ^ (k+21)->start;
								result += pivot->start ^ (k+22)->start;
								result += pivot->start ^ (k+23)->start;
								result += pivot->start ^ (k+24)->start;
								result += pivot->start ^ (k+25)->start;
								result += pivot->start ^ (k+26)->start;
								result += pivot->start ^ (k+27)->start;
								result += pivot->start ^ (k+28)->start;
								result += pivot->start ^ (k+29)->start;
								result += pivot->start ^ (k+30)->start;
								result += pivot->start ^ (k+31)->start;
								k += 32;
							}
							
							if (lastGS-k >= 16)
							{
								result += pivot->start ^ (k+0)->start;
								result += pivot->start ^ (k+1)->start;
								result += pivot->start ^ (k+2)->start;
								result += pivot->start ^ (k+3)->start;
								result += pivot->start ^ (k+4)->start;
								result += pivot->start ^ (k+5)->start;
								result += pivot->start ^ (k+6)->start;
								result += pivot->start ^ (k+7)->start;
								result += pivot->start ^ (k+8)->start;
								result += pivot->start ^ (k+9)->start;
								result += pivot->start ^ (k+10)->start;
								result += pivot->start ^ (k+11)->start;
								result += pivot->start ^ (k+12)->start;
								result += pivot->start ^ (k+13)->start;
								result += pivot->start ^ (k+14)->start;
								result += pivot->start ^ (k+15)->start;
								k += 16;
							}
							
							if (lastGS-k >= 8)
							{
								result += pivot->start ^ (k+0)->start;
								result += pivot->start ^ (k+1)->start;
								result += pivot->start ^ (k+2)->start;
								result += pivot->start ^ (k+3)->start;
								result += pivot->start ^ (k+4)->start;
								result += pivot->start ^ (k+5)->start;
								result += pivot->start ^ (k+6)->start;
								result += pivot->start ^ (k+7)->start;
								k += 8;
							}
							
							if (lastGS-k >= 4)
							{
								result += pivot->start ^ (k+0)->start;
								result += pivot->start ^ (k+1)->start;
								result += pivot->start ^ (k+2)->start;
								result += pivot->start ^ (k+3)->start;
								k += 4;
							}
							
							while (k != lastGS)
							{
								result += pivot->start ^ k->start; k++;
							}
							
							pivot++;
						}
						break;
				}
			}
			GS.clear();
		}
	}
	
	
	return result;
}


unsigned long long ParallelHashBased_ForwardScanBased_PlaneSweep_Grouping_Bucketing_Rolled(Relation *pR, Relation *pS, BucketIndex *pBIR, BucketIndex *pBIS, int runNumThreads)
{
	unsigned long long result = 0;
	int n = sqrt(runNumThreads);
	
	
	#pragma omp parallel for num_threads(runNumThreads) collapse(2) reduction(+ : result)
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			result += ForwardScanBased_PlaneSweep_Grouping_Bucketing_Rolled(pR[i], pS[j], pBIR[i], pBIS[j]);
		}
	}
	
	
	return result;
}


unsigned long long ParallelHashBased_ForwardScanBased_PlaneSweep_Grouping_Bucketing_Unrolled(Relation *pR, Relation *pS, BucketIndex *pBIR, BucketIndex *pBIS, int runNumThreads)
{
	unsigned long long result = 0;
	int n = sqrt(runNumThreads);
	
	
	#pragma omp parallel for num_threads(runNumThreads) collapse(2) reduction(+ : result)
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			result += ForwardScanBased_PlaneSweep_Grouping_Bucketing_Unrolled(pR[i], pS[j], pBIR[i], pBIS[j]);
		}
	}
	
	
	return result;
}
